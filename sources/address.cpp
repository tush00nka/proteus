#include "address.h"
#include "utils.h"

#include <algorithm>
#include <array>
#include <charconv>
#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

Address::Address(const std::string& value)
{
	std::vector<std::string> address_and_port = split(value, ':');
	if (address_and_port.size() > 1)
	{
		auto [ptr, ec] = std::from_chars(address_and_port[1].data(),
						address_and_port[1].data() + address_and_port[1].size(),
						this->_port);
		
		if (ec != std::errc())
		{
			this->_port = kDefaultPort;
		}
	}
	else
	{
		this->_port = kDefaultPort;
	}

	std::string address_string = std::move(address_and_port[0]);

	std::vector<std::string> address_parts = split(address_string, '.');
	
	auto t = [](const std::string& str) {
		uint8_t address_part = 0;

		auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), address_part);
		
		if (ec != std::errc()) // probably a bad idea, but works for now
		{
			address_part = 0;
		}

		return address_part;
	};

	std::ranges::transform(address_parts.begin(), address_parts.end(), this->_address.begin(), t);
}

// Address::Address(std::array<uint8_t, 4>& address) : _address(address) {}
Address::Address(std::array<uint8_t, 4>& address, uint16_t port) : _address(address), _port(port) {}

std::ostream& operator<<(std::ostream& out, const Address& addr)
{
	std::array<uint8_t, 4> parts = addr.getAddress();

	return out << (int)parts[0] << "." << (int)parts[1] << "." << (int)parts[2] << "." << (int)parts[3] << ":" << addr.getPort();
}
