#include "logger.h"
#include <string>
#include <utility>

#pragma once

class Options {
private:
	std::string _address;
	std::string _port;
	std::string _role;
	std::size_t _index;
	std::string _lib;

	std::string _username;

	bool _should_exit;
	int _status;

	static void usage(const std::string &program_name);
public:
	Options(int argc, char ** argv, Logger& logger);
	void errorWithMessage(const std::string& program_name, const std::string& arg, Logger& logger);

	[[nodiscard]] std::string getUsername() { return this->_username; }
	void setUsername(std::string username) { this->_username = std::move(username); }

	[[nodiscard]] bool getShouldExit() const { return this->_should_exit; }
	void setShouldExit() { this->_should_exit = !this->_should_exit; }
	
	[[nodiscard]] int getStatus() const { return this->_status; }
	void setStatus(int status) { this->_status = status; }
};
