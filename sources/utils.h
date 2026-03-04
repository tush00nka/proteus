#include <any>
#include <ostream>
#include <vector>

#pragma once

std::ostream& operator<<(std::ostream& os, const std::any& value);
std::vector<std::string> split(const std::string& value, char delim);