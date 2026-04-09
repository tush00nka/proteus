#include <condition_variable>
#include <cstdint>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <unistd.h>
#include <arpa/inet.h>

#include "../logger.h"
#include "enodeb.h"
#include "mme.h"

const int kPort = 8080;
const int kBufferSize = 1024;
const int kMaxClients = 4;

class ThreadPool {
private:
    std::vector<std::thread> _workers;
    std::queue<std::function<void()>> _tasks;
    std::mutex _queueMutex;
    std::condition_variable _condition;
    bool _stop = false;
public:
    explicit ThreadPool(size_t numThreads)
	{
        for (size_t i = 0; i < numThreads; ++i)
		{
            _workers.emplace_back([this]
			{
                while (true)
				{
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(_queueMutex);
                        _condition.wait(lock, [this]
						{ 
                            return _stop || !_tasks.empty(); 
                        });
                        
                        if (_stop && _tasks.empty())
						{
                            return;
                        }
                        
                        task = std::move(_tasks.front());
                        _tasks.pop();
                    }
                    task();
                }
            });
        }
    }
    
    template<class F>
    void enqueue(F&& f)
	{
        {
            std::unique_lock<std::mutex> lock(_queueMutex);
            _tasks.emplace(std::forward<F>(f));
        }

        _condition.notify_one();
    }
    
    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(_queueMutex);
            _stop = true;
        }
        _condition.notify_all();
        for (std::thread &worker : _workers) {
            worker.join();
        }
    }
};

class ClientHandler {
private:
    int _client_fd;
    std::shared_ptr<MME> _mme;
    std::shared_ptr<std::vector<std::shared_ptr<ENodeB>>> _enbs; // the so-called баребух
    TMSI _my_tmsi = 0;
    int _current_enodeb_id = -1;
    std::mutex _mux;
public:
    ClientHandler(int clientFd, MME& mme, std::vector<std::shared_ptr<ENodeB>>& enbs)
        : _client_fd(clientFd), _mme(std::make_shared<MME>(mme)), _enbs(std::make_shared<std::vector<std::shared_ptr<ENodeB>>>(enbs)) {}
    
    void handle()
	{
        std::array<char, kBufferSize> buffer = {0};
        
        while (true)
		{
            memset(buffer.data(), 0, kBufferSize);
            
            int64_t bytes_read = read(_client_fd, buffer.data(), kBufferSize - 1);
            
            if (bytes_read <= 0)
			{
                if (bytes_read == 0)
				{
                    log<LogLevel::INFO>("Client disconnected");
                    std::cout << "Client disconnected\n";
                } else
				{
                    log<LogLevel::ERROR>("Read error");
                    std::cerr << "Read error\n";
                }
                break;
            }
            
            const std::string msg = std::format("Received: {}", buffer.data());
            log<LogLevel::INFO>(msg);

            std::string_view sv(buffer.data());

            auto parts = split(sv, '|');

            if (parts.empty())
            {
                continue;
            }

            // R|IMSI|POS
            if (parts[0] == "R")
            {
                IMSI imsi = std::stoull(std::string(parts[1]));  // TODO(tushonka): handle exception properly
                int position = std::stoi(std::string(parts[2])); // TODO(tushonka): handle exception properly

                std::string response;
                for (const auto& enb : *_enbs)
                {
                    double signal_stength = enb->signalStrength(position);
                    response += std::format("{}|{}|", enb->_id, signal_stength);
                }
                send(_client_fd, response.c_str(), response.length(), 0);
                log<LogLevel::INFO>(std::format("Sent: {}", response));
                continue;
            }

            // A|IMSI|IMEI|MSISDN|enb_id
            if (parts[0] == "A")
            {
                IMSI imsi = std::stoull(std::string(parts[1]));      // TODO(tushonka): handle exception properly
                IMEI imei = std::stoull(std::string(parts[2]));      // TODO(tushonka): handle exception properly
                MSISDN msisdn = std::stoull(std::string(parts[3]));  // TODO(tushonka): handle exception properly
                int enb_id = std::stoi(std::string(parts[4]));       // TODO(tushonka): handle exception properly
                
                std::lock_guard<std::mutex> lock(_mux);
                
                // TODO(tushonka) not sure if we need the position at this point
                _my_tmsi = _mme->registerSubscriber(imsi, imei, msisdn, enb_id, 0, _client_fd); 

                std::string response = "TMSI|" + std::to_string(_my_tmsi);
                send(_client_fd, response.c_str(), response.length(), 0);
                log<LogLevel::INFO>(std::format("Sent: {}", response));
                continue;
            }

            if (parts[0] == "OFF")
            {
                log<LogLevel::DEBUG>(std::format("parts[0]: {}", parts[0]));

                std::lock_guard<std::mutex> lock(_mux);
                _mme->unregister(_my_tmsi);
                std::string response = "OFF_OK";
                send(_client_fd, response.c_str(), response.length(), 0);
                log<LogLevel::INFO>(std::format("Sent: {}", response));
                break;
            }

            // TODO(tushonka): other messages here
        }
        
        close(_client_fd);
        log<LogLevel::DEBUG>("Closed client socket");
    }
};

const std::uint32_t kENodeBRadius = 120;
const int kENodeBPosition = 100;

int main()
{
	int server_fd = 0;
	struct sockaddr_in address {};
	int addrlen = sizeof(address);
	std::array<char, kBufferSize> buffer = {0};

	Logger::init("proteus_server.log", true);
	Logger::clear("proteus_server.log");

    MME mme;
    std::shared_ptr<ENodeB> enodeb1 =  std::make_shared<ENodeB>(1, -kENodeBPosition, kENodeBRadius);
    std::shared_ptr<ENodeB> enodeb2 =  std::make_shared<ENodeB>(2, kENodeBPosition, kENodeBRadius);
    std::vector<std::shared_ptr<ENodeB>> enbs = {enodeb1, enodeb2};

	int socket_creation_result = (server_fd = socket(AF_INET, SOCK_STREAM, 0));
	if (socket_creation_result == 0)
	{
		log<LogLevel::FATAL>("Socket creation failed");
        std::cerr << "Socket creation failed\n";
        return -1;
    }

	int opt = 1;
	int setsockopt_result = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (setsockopt_result < 0)
	{
		log<LogLevel::FATAL>("Setsockopt failed");
        std::cerr << "Setsockopt failed\n";
        return -1;
    }

	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(kPort);

	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	if (bind(server_fd, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)) < 0)
	{
		log<LogLevel::FATAL>("Bind failed");
        std::cerr << "Bind failed\n";
        return -1;
    }

	if (listen(server_fd, 3) < 0)
	{
		log<LogLevel::FATAL>("Listen failed");
        std::cerr << "Listen failed\n";
        return -1;
    }

	log<LogLevel::INFO>(std::format("Server listening on port {}", kPort));
	std::cout << "Server listening on port " << kPort << "\n";

	ThreadPool thread_pool(kMaxClients);
    std::atomic<int> active_connections{0};
	std::mutex cout_mutex;

    while (true)
	{
		int client_fd = 0;

        int client_accept_result = (
			// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast) 
            client_fd = accept(server_fd, reinterpret_cast<struct sockaddr*>(&address), reinterpret_cast<socklen_t*>(&addrlen))
        );
        
        if (client_accept_result < 0) {
            log<LogLevel::ERROR>("Accept failed");
            std::cerr << "Accept failed\n";
            continue;
        }

        if (active_connections >= kMaxClients) {
            log<LogLevel::WARNING>("Maximum clients reached, rejecting connection");
            std::cerr << "Maximum clients reached, rejecting connection\n";
            const char* msg = "Server busy, maximum clients reached";
            send(client_fd, msg, strlen(msg), 0);
            close(client_fd);
            continue;
        }

		active_connections++;
        log<LogLevel::INFO>(std::format("Client connected. Active connections: {}", active_connections.load()));
        std::cout << "Client connected! Active connections: " << active_connections<< "\n";
        
		thread_pool.enqueue([client_fd, &mme, &enbs, &active_connections, &cout_mutex]() {
            ClientHandler handler(client_fd, mme, enbs);
            handler.handle();
            
            active_connections--;
            log<LogLevel::INFO>(std::format("Client disconnected. Active connections: {}", active_connections.load()));
            {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << "Client disconnected. Active connections: " << active_connections << "\n";
            }
        });
    }

	close(server_fd);
	log<LogLevel::DEBUG>("Closed server socket");

	return 0;
}