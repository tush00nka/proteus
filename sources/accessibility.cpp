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

bool ConnectionTest::check(const std::string& address, std::vector<std::string>& ports)
{
	auto addr = Address(address);
	
	int sock = 0;
	struct sockaddr_in serv_addr{};

	int sock_creation_result = (sock = socket(AF_INET, SOCK_STREAM, 0));
	if (sock_creation_result < 0) {
		return false;
	}

	for (int i = 0; i < ports.size(); ++i)
	{
		addr.setPort(std::stoi(ports[i]));

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(addr.getPort());

		int inet_pton_result = inet_pton(AF_INET, addr.getAddressString().data(), &serv_addr.sin_addr);
		if (inet_pton_result < 0)
		{
			const std::string& msg = "Invalid address / Address not supported";
			close(sock);
			return false;
		}

		int sock_connection_result = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		if (sock_creation_result < 0)
		{
			close(sock);
			return false;
		}	
	}

	close(sock);
	
	return true;
}

bool ResourceTest::check(const std::string& path, std::vector<std::string>&  filenames) 
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