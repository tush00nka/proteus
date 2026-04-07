// TODO(tush00nka): move implementation to .cpp file

#pragma once

#include "address.h"
#include "logger.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <unistd.h>

const int kBufferSize = 1024; 

class TCPClient {
private:
	int _sock {-1};
public:
	TCPClient() = default;
	TCPClient(const TCPClient &) = default;
	TCPClient(TCPClient &&) = delete;
	TCPClient &operator=(const TCPClient &) = default;
	TCPClient &operator=(TCPClient &&) = delete;
	~TCPClient() { disconnect(); }

    bool connect(Address &address)
	{
		_sock = socket(AF_INET, SOCK_STREAM, 0); 
		if (_sock < 0)
		{
			log<LogLevel::ERROR>("Failed to create socket");
			return false;
		}

		struct sockaddr_in serv_addr {};
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(address.getPort());

		const std::string address_string = address.getAddressString();

		if (inet_pton(AF_INET, address_string.c_str(), &serv_addr.sin_addr) <= 0) {
			log<LogLevel::ERROR>("Invalid address: " + address_string);
            disconnect();
            return false;
        }

		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
		if (::connect(_sock, reinterpret_cast<struct sockaddr*>(&serv_addr), sizeof(serv_addr)) < 0) {
			log<LogLevel::ERROR>("Connection failed to " + address_string + ":" + std::to_string(address.getPort()));	
			disconnect();
            return false;
        }

		log<LogLevel::INFO>("Connected to " + address_string + ":" + std::to_string(address.getPort()));	
		return true;
	}

	void disconnect()
	{
		if (_sock != -1)
		{
			close(_sock);
			_sock = -1;
		}
	}

	bool sendMessage(const std::string& message) const
	{
		if (_sock < 0)
		{
			log<LogLevel::ERROR>("Not connected to server");
			return false;
		}

		ssize_t bytes_sent = send(_sock, message.c_str(), message.length(), 0);
		if (bytes_sent < 0)
		{
			log<LogLevel::ERROR>("Failed to send message");
			return false;
		}

		log<LogLevel::INFO>("Sent " + std::to_string(bytes_sent) + " bytes: " + message);

		return true;
	}

	std::string receiveResponse() {
        if (_sock == -1) {
            return "";
        }
        
        char buffer[kBufferSize] = {0};
        
        ssize_t bytes_read = read(_sock, static_cast<void*>(buffer), kBufferSize - 1);
        
        if (bytes_read < 0) {
			log<LogLevel::ERROR>("Failed to recieve response");
			return "";
        } 
		
		if (bytes_read == 0) {
			log<LogLevel::ERROR>("Server disconnected");
            disconnect();
            return "";
        }
        
		std::string response(static_cast<char*>(buffer), bytes_read); 
        return response;
    }

	std::string sendAndReceive(const std::string& message)
	{
		if (sendMessage(message))
		{
			std::string response = receiveResponse(); 
			log<LogLevel::INFO>("Got response from server: " + response);
			return response;
		}

		return "";
	}
};