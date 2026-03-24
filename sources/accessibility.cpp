#include "accessibility.h"
#include "address.h"
#include "logger.h"
#include "utils.h"

#include <algorithm>
#include <filesystem>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <vector>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

template<typename AddrType>
static int connectWrapper(int sock, const AddrType& addr)
{
	// we have to use reinterpret_cast here as we work with raw C pointers 
	// 
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	return connect(sock, reinterpret_cast<const struct sockaddr*>(&addr), sizeof(addr));
}

// if at least one port is available, we return true, otherwise false
bool ConnectionTest::check(std::string_view address, std::vector<std::string>& ports)
{
	auto addr = Address(address);
	
	for (const auto & port : ports)
	{
		int sock = 0;
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock < 0)
		{
			return false;
		}

		struct timeval timeout{};
		timeout.tv_sec = 2;
		timeout.tv_usec = 0;
		setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
		setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

		addr.setPort(std::stoi(port));

		struct sockaddr_in serv_addr{};
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(addr.getPort());

		int inet_pton_result = inet_pton(AF_INET, addr.getAddressString().data(), &serv_addr.sin_addr);
		if (inet_pton_result <= 0)
		{
			const std::string_view msg = "Invalid address / Address not supported";
			log<LogLevel::WARNING>(msg);
			close(sock);
			continue;
		}

		int sock_connection_result = connectWrapper(sock, serv_addr);
		if (sock_connection_result == 0)
		{
			log<LogLevel::INFO>("Connected to: " + addr.getAddressString());
			close(sock);
			return true;
		}	
		
		log<LogLevel::DEBUG>("Failed to connect to port: " + port);
		close(sock);
	}

	return false;
}

bool ResourceTest::check(std::string_view path, std::vector<std::string>& filenames) 
{
	std::unordered_map<std::string, size_t> appearances; 

	for (const auto & entry : std::filesystem::directory_iterator(path))
	{
        const std::string full_path = entry.path();		
		std::vector<std::string> tokens = split(full_path, '/');
		
		appearances[tokens[tokens.size()-1]]++;
	}
	
	auto has_all_files = [appearances](const std::string& f){
		return appearances.contains(f);
	};

	return std::ranges::all_of(
		filenames.cbegin(),
		filenames.cend(),
		has_all_files
	);
}