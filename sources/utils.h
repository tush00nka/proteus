#pragma once

#include <any>
#include <ostream>
#include <vector>

std::ostream& operator<<(std::ostream& os, const std::any& value);
std::vector<std::string_view> split(std::string_view sv, char delim);

std::string ltrim(std::string_view str, std::string_view toTrim = " \t\n\r\f\v");
std::string rtrim(std::string_view str, std::string_view toTrim = " \t\n\r\f\v");
std::string trim (std::string_view str, std::string_view toTrim = " \t\n\r\f\v");
std::string toUpper(std::string& str);

using IMEI = std::uint64_t;
using IMSI = std::uint64_t;
using MSISDN = std::uint64_t;
using TMSI = std::int64_t;
