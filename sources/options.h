#include "address.h"

#pragma once

class Options {
private:
	Address _address; // _port is included in Address class
	std::string _msisdn;
	std::string _imei;
	std::string _imsi;
	int _position = 0; // one-dimensional 

	std::string _username;

	static void usage(std::string_view program_name);
public:
	Options() = default;
	bool parseArguments(int argc, char ** argv);
	static void errorWithMessage(std::string_view program_name, std::string_view message);

	[[nodiscard]] std::string_view getUsername() const { return this->_username; }
	void setUsername(std::string_view username) { this->_username = username; }

	[[nodiscard]] Address getAddress() const;
	
	// bool just in case if we need to make checks or ensure constraints
	bool setPosition(int newPosition);
	int getPosition() const { return this->_position; }
};
