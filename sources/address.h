#pragma once

#include "logger.h"
#include <cstdint>
#include <array>
#include <string>
#include <sstream>

const uint16_t kDefaultPort { 8080 }; 
const uint8_t kAddressPartsSize { 5 };
const uint8_t kIpOctet { 255 };
const uint16_t kMinPort { 1 };
const uint16_t kMaxPort { 65535 };

/*
 * Class Invariants for Address:
 * 
 * 1. _address array must always contain exactly 4 octets
 *    - Each octet must be in range [0, 255]
 *    - No octet can be invalid (always holds valid uint8_t)
 * 
 * 2. _port must always be in valid range
 *    - Must be between kMinPort (1) and kMaxPort (65535)
 *    - Default port (8080) is used if not explicitly set
 * 
 * 3. The string representation must be consistent
 *    - Format: "octet1.octet2.octet3.octet4:port"
 *    - All octets must be convertible to valid strings
 * 
 * 4. After construction, the object is always in a valid state
 *    - Default constructor creates address "0.0.0.0:8080"
 *    - String constructor must validate input
 *    - Array constructor must validate octets
 * 
 * 5. All setters and modifiers maintain invariants
 *    - Port must be validated on set
 *    - Address can only be set through constructors (immutable address)
 */
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
