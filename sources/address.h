#pragma once

#include "logger.h"
#include <cstdint>
#include <array>
#include <string>
#include <sstream>

const uint16_t kDefaultPort { 8080 }; 
const uint8_t kAddressPartsSize { 5 };
const uint8_t kIpOctet { 255 };

class Address {

private:
	uint16_t _port {};
	std::array<uint8_t, 4> _address {};

public:
	Address() = default;
	explicit Address(const std::string& value, Logger& logger);
	explicit Address(std::array<uint8_t, 4>&, uint16_t port = kDefaultPort);

	void setPort(uint16_t port)
	{
		this->_port = port;
	}

	Address& withPort(uint16_t port)
	{
		this->_port = port;
		return *this;
	}

	std::string sprint()
	{
		std::ostringstream oss;
		oss << (int)this->_address[0] << "." << (int)this->_address[1] << "." << (int)this->_address[2] << "." << (int)this->_address[3] << ":" << this->_port;
		return oss.str();
	}

	[[nodiscard]] uint16_t getPort() const
	{
		return this->_port;
	}

	[[nodiscard]] std::array<uint8_t, 4> getAddress() const
	{
		return this->_address;
	}

	[[nodiscard]] std::string getAddressString() const
	{
		std::ostringstream oss;
		oss << (int)this->_address[0] << "." << (int)this->_address[1] << "." << (int)this->_address[2] << "." << (int)this->_address[3];
		return oss.str();
	}
};

std::ostream& operator<<(std::ostream& out, const Address& addr);
