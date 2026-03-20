#include <any>
#include <ostream>
#include <vector>

#pragma once

std::ostream& operator<<(std::ostream& os, const std::any& value);
std::vector<std::string> split(std::string_view value, char delim);

std::string ltrim(std::string_view str, std::string_view toTrim = " \t\n\r\f\v");
std::string rtrim(std::string_view str, std::string_view toTrim = " \t\n\r\f\v");
std::string trim (std::string_view str, std::string_view toTrim = " \t\n\r\f\v");
std::string toUpper(std::string& str);