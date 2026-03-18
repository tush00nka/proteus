#include <cstdint>
#include <cstring>
#include <iostream>
#include "vector4.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

const int kPort = 8080;
const int kBufferSize = 1024;

int main()
{
	int server_fd = 0;
	int client_fd = 0;
	struct sockaddr_in address {};
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[kBufferSize] = {0};

	// std::unordered_map<std::string, std::function<void(Vector4& input)>> processors = {
	// 	{"bool", [](Vector4& vec){ vec.setData(vec., const std::string &yStr, const std::string &zStr, const std::string &wStr)}},	
	// };

	int socket_creation_result = (server_fd = socket(AF_INET, SOCK_STREAM, 0));
	if (socket_creation_result == 0) {
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
        std::cerr << "Bind failed\n";
        return -1;
    }

	if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed\n";
        return -1;
    }

	std::cout << "Server listening on port " << kPort << "\n";

	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	int client_accept_result = (client_fd = accept(server_fd, reinterpret_cast<struct sockaddr*>(&address), reinterpret_cast<socklen_t*>(&addrlen)));
	if (client_accept_result < 0) {
        std::cerr << "Accept failed\n";
        return -1;
    }
    
    std::cout << "Client connected!\n";

    while (true) {
        memset(static_cast<void*>(buffer), 0, kBufferSize);
        
        int64_t bytes_read = read(client_fd, static_cast<void*>(buffer), kBufferSize - 1);
        
        if (bytes_read <= 0) {
            if (bytes_read == 0) {
                std::cout << "Client disconnected\n";
            } else {
                std::cerr << "Read error\n";
            }
            break;
        }
        
        std::cout << "Received: " << static_cast<char*>(buffer) << "\n";
        
		Vector4 vec;
		vec.deserialize(std::string(static_cast<char*>(buffer)));

		// perform data processing here

        std::string response = vec.serialize();
        send(client_fd, response.c_str(), response.length(), 0);
    }

	close(client_fd);
	close(server_fd);

	return 0;
}