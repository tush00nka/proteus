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
	auto x = std::to_string(std::any_cast<T>(vec.getX()) + 2);
	auto y = std::to_string(std::any_cast<T>(vec.getY()) - 2);
	auto z = std::to_string(std::any_cast<T>(vec.getZ()) * 2);
	auto w = std::to_string(std::any_cast<T>(vec.getW()) / 2);	
	vec.setData(x, y, z, w);
}

int main()
{
	int server_fd = 0;
	int client_fd = 0;
	struct sockaddr_in address {};
	int opt = 1;
	int addrlen = sizeof(address);
	std::array<char, kBufferSize> buffer = {0};

	Logger logger("proteus_server.log", true);
	logger.clear("proteus_server.log");

	std::unordered_map<std::string, std::function<void(Vector4& input)>> processors = {
		{"bool", 
			[](Vector4& vec)
			{
				std::string x = std::to_string(!std::any_cast<bool>(vec.getX()));
				std::string y = std::to_string(!std::any_cast<bool>(vec.getY()));
				std::string z = std::to_string(!std::any_cast<bool>(vec.getZ()));
				std::string w = std::to_string(!std::any_cast<bool>(vec.getW()));
				vec.setData(x, y, z, w);
			}
		},
		{"string", 
			[](Vector4& vec)
			{
				auto x = std::any_cast<std::string>(vec.getX());
				toUpper(x);
				auto y = std::any_cast<std::string>(vec.getY());
				toUpper(y);
				auto z = std::any_cast<std::string>(vec.getZ());
				toUpper(z);
				auto w = std::any_cast<std::string>(vec.getW());
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
		log<LogLevel::FATAL>(logger, "Socket creation failed");
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
		log<LogLevel::FATAL>(logger, "Bind failed");
        std::cerr << "Bind failed\n";
        return -1;
    }

	if (listen(server_fd, 3) < 0) {
		log<LogLevel::FATAL>(logger, "Listen failed");
        std::cerr << "Listen failed\n";
        return -1;
    }

	log<LogLevel::INFO>(logger, std::format("Server listening on port {}", kPort));
	std::cout << "Server listening on port " << kPort << "\n";

	int client_accept_result = (
		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
		client_fd = accept(server_fd, reinterpret_cast<struct sockaddr*>(&address), reinterpret_cast<socklen_t*>(&addrlen))
	);
	if (client_accept_result < 0) {
		log<LogLevel::FATAL>(logger, "Accept failed");
        std::cerr << "Accept failed\n";
        return -1;
    }
    
	log<LogLevel::INFO>(logger, "Client connected");
    std::cout << "Client connected!\n";

    while (true) {
        memset(buffer.data(), 0, kBufferSize);
        
        int64_t bytes_read = read(client_fd, buffer.data(), kBufferSize - 1);
        
        if (bytes_read <= 0) {
            if (bytes_read == 0) {
				log<LogLevel::INFO>(logger, "Client connected");
                std::cout << "Client disconnected\n";
            } else {
				log<LogLevel::ERROR>(logger, "Read error");
                std::cerr << "Read error\n";
            }
            break;
        }
        
		log<LogLevel::DEBUG>(logger, std::format("Recieved: {}", buffer.data()));
        std::cout << "Received: " << buffer.data() << "\n";
        
		Vector4 vec;
		vec.deserialize(std::string(buffer.data()));

		// process vector according to type
		processors[vec.getType()](vec);

        std::string response = vec.serialize();
        send(client_fd, response.c_str(), response.length(), 0);

		log<LogLevel::DEBUG>(logger, std::format("Sent: {}", response));
        std::cout << "Sent: " << response << "\n";

    }

	close(client_fd);
	log<LogLevel::DEBUG>(logger, "Closed client socket");
	close(server_fd);
	log<LogLevel::DEBUG>(logger, "Closed server socket");

	return 0;
}