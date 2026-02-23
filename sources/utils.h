#include <any>
#include <string>

#pragma once

void usage(std::string &programName);
std::ostream& operator<<(std::ostream& os, const std::any& value);