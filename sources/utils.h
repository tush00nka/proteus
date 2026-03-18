#include <any>
#include <ostream>
#include <vector>

#pragma once

std::ostream& operator<<(std::ostream& os, const std::any& value);
std::vector<std::string> split(const std::string& value, char delim);

std::string ltrim(const std::string& str, const std::string& toTrim = " \t\n\r\f\v");
std::string rtrim(const std::string& str, const std::string& toTrim = " \t\n\r\f\v");
std::string trim (const std::string& str, const std::string& toTrim = " \t\n\r\f\v");
std::string toUpper(const std::string& str);