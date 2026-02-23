#include <string>
#include <iostream>

#pragma once

class Options {
public:
	std::string address;
	std::string port;
	std::string role;
	std::size_t i;
	std::string lib;

	Options(int argc, char ** argv);
	// ~Options();
};
