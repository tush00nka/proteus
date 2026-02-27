#include <any>
#include <iostream>
#include "utils.h"

void usage(std::string &programName)
{
	std::cout << "USAGE:\n";
	std::cout << programName << " [OPTIONS]" << "\n";
	std::cout << "\t" << "-a ADDRESS" << "\n";
	std::cout << "\t" << "-p PORT" << "\n";
	std::cout << "\t" << "-r ROLE" << "\n";
	std::cout << "\t" << "-i INDEX" << "\n";
	std::cout << "\t" << "-L LIB" << "\n";
}

std::ostream& operator<<(std::ostream& os, const std::any& value) {
    if (!value.has_value()) {
        return os << "null";
    }
    
    if (value.type() == typeid(int)) {
        return os << std::any_cast<int>(value);
    }
    if (value.type() == typeid(unsigned int)) {
        return os << std::any_cast<unsigned int>(value);
    }
    if (value.type() == typeid(std::int64_t)) {
        return os << std::any_cast<std::int64_t>(value);
    }
    if (value.type() == typeid(float)) {
        return os << std::any_cast<float>(value);
    }
    if (value.type() == typeid(double)) {
        return os << std::any_cast<double>(value);
    }
    if (value.type() == typeid(char)) {
        return os << "'" << std::any_cast<char>(value) << "'";
    }
    if (value.type() == typeid(std::string)) {
        return os << "\"" << std::any_cast<std::string>(value) << "\"";
    }
    if (value.type() == typeid(const char*)) {
        return os << "\"" << std::any_cast<const char*>(value) << "\"";
    }
    if (value.type() == typeid(bool)) {
        return os << (std::any_cast<bool>(value) ? "true" : "false");
    }
    
    return os << "unknown_type(" << value.type().name() << ")";
}