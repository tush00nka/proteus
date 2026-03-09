#include "accessibility.h"
#include "address.h"
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

bool ConnectionTest::check(const std::string& address, std::vector<std::string>& ports, Logger& logger)
{
	auto addr = Address(address, logger);
	
	int sock = 0;
	struct sockaddr_in serv_addr{};

	int sock_creation_result = (sock = socket(AF_INET, SOCK_STREAM, 0));
	if (sock_creation_result < 0) {
		return false;
	}

	for (const auto & port : ports)
	{
		addr.setPort(std::stoi(port));

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(addr.getPort());

		int inet_pton_result = inet_pton(AF_INET, addr.getAddressString().data(), &serv_addr.sin_addr);
		if (inet_pton_result < 0)
		{
			const std::string& msg = "Invalid address / Address not supported";
			close(sock);
			return false;
		}

		int sock_connection_result = connectWrapper(sock, serv_addr);
		if (sock_creation_result < 0)
		{
			close(sock);
			return false;
		}	
	}

	close(sock);
	
	return true;
}

bool ResourceTest::check(const std::string& path, std::vector<std::string>& filenames, Logger&  /*logger*/) 
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