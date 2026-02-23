#include <iostream>
#include "utils.h"

void usage(std::string &programName)
{
	std::cout << "USAGE:\n";
	std::cout << programName << " [OPTIONS]" << "\n";
	std::cout << "\t" << "-a ADDRESS" << "\n";
	std::cout << "\t" << "-p PORT" << "\n";
	std::cout << "\t" << "-r ROLE" << "\n";
	std::cout << "\t" << "-i ???" << "\n";
	std::cout << "\t" << "-L LIB" << "\n";
}