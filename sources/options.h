#include <string>
#include <iostream>

#pragma once

class Options {
private:
	std::string address;
	std::string port;
	std::string role;
	std::size_t i;
	std::string lib;
public:
	Options(int argc, char ** argv);
	// ~Options();
	std::string GetRole() { return this->role; }
	void SetRole(std::string role) { this->role = role; }
};
