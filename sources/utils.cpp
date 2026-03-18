#include <any>
#include <iostream>
#include <sstream>
#include "utils.h"

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

std::vector<std::string> split(const std::string& value, char delim) {
	std::vector<std::string> tokens {};

	std::istringstream token_stream(value);
	std::string token;
	while(std::getline(token_stream, token, delim))
	{
		tokens.push_back(token);
	}

	return tokens;
}

// Trim from start (left)
std::string ltrim(const std::string& str, const std::string& toTrim) {
    size_t start = str.find_first_not_of(toTrim);
    return (start == std::string::npos) ? "" : str.substr(start);
}

// Trim from end (right)
std::string rtrim(const std::string& str, const std::string& toTrim) {
    size_t end = str.find_last_not_of(toTrim);
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}

// Trim from both ends
std::string trim(const std::string& str, const std::string& toTrim) {
    return rtrim(ltrim(str, toTrim), toTrim);
}