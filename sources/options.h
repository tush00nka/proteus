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

	bool _should_exit;
	int _status;
public:
	Options(int argc, char ** argv);
	// ~Options();
	std::string getRole() { return this->_role; }
	[[nodiscard]] bool getShouldExit() const { return this->_should_exit; }

	void setRole(std::string role) { this->_role = std::move(role); }
	void setShouldExit() { this->_should_exit = !this->_should_exit; }
	
	[[nodiscard]] int getStatus() const { return this->_status; }
	void setStatus(int status) { this->_status = status; }
};
