#include "address.h"
#include "logger.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <exception>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

Address::Address(const std::string& value, Logger& logger)
{
	std::string str = value;
	std::ranges::replace(str, '.', ' ');
	std::ranges::replace(str, ':', ' ');

	std::vector<int> parts;
	std::istringstream iss(str);
	int num { 0 };
	try {
		while (iss >> num)
		{
			parts.push_back(num);
		}
	}
	catch (std::exception& e)
	{
		log<LogLevel::FATAL>(logger, "Failed to parse address string");
		return;
	}

	if (parts.size() >= kAddressPartsSize)
	{
		this->_port = parts[parts.size()-1];
		parts.pop_back();
	}
	else
	{
		this->_port = kDefaultPort;
	}

	if (parts[0] < 0 || parts[0] > kIpOctet ||
		parts[1] < 0 || parts[1] > kIpOctet ||
		parts[2] < 0 || parts[2] > kIpOctet ||
		parts[3] < 0 || parts[3] > kIpOctet)
	{
		_address = std::array<uint8_t, 4> {0,0,0,0};
		log<LogLevel::ERROR>(logger, "IP octet out of range (0-255)");
		return;
	}

	_address = std::array<uint8_t, 4>{
		static_cast<uint8_t>(parts[0]),
		static_cast<uint8_t>(parts[1]),
		static_cast<uint8_t>(parts[2]),
		static_cast<uint8_t>(parts[3])
	};
}

// Address::Address(std::array<uint8_t, 4>& address) : _address(address) {}
Address::Address(std::array<uint8_t, 4>& address, uint16_t port) : _address(address), _port(port) {}

std::ostream& operator<<(std::ostream& out, const Address& addr)
{
	std::array<uint8_t, 4> parts = addr.getAddress();

	return out << (int)parts[0] << "." << (int)parts[1] << "." << (int)parts[2] << "." << (int)parts[3] << ":" << addr.getPort();
}
