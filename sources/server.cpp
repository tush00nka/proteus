#include <cstdint>
#include <cstring>
#include <functional>
#include <iostream>
#include "logger.h"
#include "utils.h"
#include "vector4.h"

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <unordered_map>

const int kPort = 8080;
const int kBufferSize = 1024;

template <typename T>
static void doMath(Vector4& vec)
{
	auto data = vec.getData();
	auto x = std::to_string(std::any_cast<T>(data[0]) + 2);
	auto y = std::to_string(std::any_cast<T>(data[1]) - 2);
	auto z = std::to_string(std::any_cast<T>(data[2]) * 2);
	auto w = std::to_string(std::any_cast<T>(data[3]) / 2);	
	vec.setData(x, y, z, w);
}

int main()
{
	int server_fd = 0;
	int client_fd = 0;
	struct sockaddr_in address {};
	int addrlen = sizeof(address);
	std::array<char, kBufferSize> buffer = {0};

	Logger::init("proteus_server.log", true);
	Logger::clear("proteus_server.log");

	std::unordered_map<std::string, std::function<void(Vector4& input)>> processors = {
		{"bool", 
			[](Vector4& vec)
			{
				auto data = vec.getData();
				std::string x = std::to_string(!std::any_cast<bool>(data[0]));
				std::string y = std::to_string(!std::any_cast<bool>(data[1]));
				std::string z = std::to_string(!std::any_cast<bool>(data[2]));
				std::string w = std::to_string(!std::any_cast<bool>(data[3]));
				vec.setData(x, y, z, w);
			}
		},
		{"string", 
			[](Vector4& vec)
			{
				auto data = vec.getData();
				auto x = std::any_cast<std::string>(data[0]);
				toUpper(x);
				auto y = std::any_cast<std::string>(data[1]);
				toUpper(y);
				auto z = std::any_cast<std::string>(data[2]);
				toUpper(z);
				auto w = std::any_cast<std::string>(data[3]);
				toUpper(w);
				vec.setData(x, y, z, w);
			}
		},
		{"char", doMath<char>},
		{"int", doMath<int>},
		{"uint", doMath<uint>},
		{"float", doMath<float>},
		{"double", doMath<double>},
	};

	int socket_creation_result = (server_fd = socket(AF_INET, SOCK_STREAM, 0));
	if (socket_creation_result == 0) {
		log<LogLevel::FATAL>("Socket creation failed");
        std::cerr << "Socket creation failed\n";
        return -1;
    }
	
	// int setsockopt_result = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    // if (setsockopt_result == 0) {
    //     std::cerr << "Setsockopt failed\n";
    //     return -1;
    // }

	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(kPort);

	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	if (bind(server_fd, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)) < 0) {
		log<LogLevel::FATAL>("Bind failed");
        std::cerr << "Bind failed\n";
        return -1;
    }

	if (listen(server_fd, 3) < 0) {
		log<LogLevel::FATAL>("Listen failed");
        std::cerr << "Listen failed\n";
        return -1;
    }

	log<LogLevel::INFO>(std::format("Server listening on port {}", kPort));
	std::cout << "Server listening on port " << kPort << "\n";

	int client_accept_result = (
		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
		client_fd = accept(server_fd, reinterpret_cast<struct sockaddr*>(&address), reinterpret_cast<socklen_t*>(&addrlen))
	);
	if (client_accept_result < 0) {
		log<LogLevel::FATAL>("Accept failed");
        std::cerr << "Accept failed\n";
        return -1;
    }
    
	log<LogLevel::INFO>("Client connected");
    std::cout << "Client connected!\n";

    while (true) {
        memset(buffer.data(), 0, kBufferSize);
        
        int64_t bytes_read = read(client_fd, buffer.data(), kBufferSize - 1);
        
        if (bytes_read <= 0) {
            if (bytes_read == 0) {
				log<LogLevel::INFO>("Client connected");
                std::cout << "Client disconnected\n";
            } else {
				log<LogLevel::ERROR>("Read error");
                std::cerr << "Read error\n";
            }
            break;
        }
        
		log<LogLevel::DEBUG>(std::format("Recieved: {}", buffer.data()));
        std::cout << "Received: " << buffer.data() << "\n";
        
		Vector4 vec;
		vec.deserialize(std::string(buffer.data()));

		// process vector according to type
		processors[vec.getType()](vec);

        std::string response = vec.serialize();
        send(client_fd, response.c_str(), response.length(), 0);

		log<LogLevel::DEBUG>(std::format("Sent: {}", response));
        std::cout << "Sent: " << response << "\n";

    }

	close(client_fd);
	log<LogLevel::DEBUG>("Closed client socket");
	close(server_fd);
	log<LogLevel::DEBUG>("Closed server socket");

	return 0;
}