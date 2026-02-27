#include "vector4.h"
#include "console_interface.h"
#include "utils.h"

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

Vector4::Vector4(): _type("int"), _x(0), _y(0), _z(0), _w(0) {
		this->_supported_types = {"int",  "uint",   "float", "double", "char", "string", "bool"};
}

template<typename T>
T Vector4::convertTo(const std::string& str) const {
	if constexpr (std::is_same_v<T, int>) {
		return std::stoi(str);
	}
	else if constexpr (std::is_same_v<T, unsigned int>) {
		return static_cast<unsigned int>(std::stoul(str));
	}
	else if constexpr (std::is_same_v<T, float>) {
		return std::stof(str);
	}
	else if constexpr (std::is_same_v<T, double>) {
		return std::stod(str);
	}
	else if constexpr (std::is_same_v<T, char>) {
		if (str.empty()) return '\0';
		return str[0];
	}
	else if constexpr (std::is_same_v<T, bool>) {
		return str == "true" || str == "1" || str == "yes";
	}
	else if constexpr (std::is_same_v<T, std::string>) {
		return str;
	}
	else {
		return T{};
	}
}

bool Vector4::supportsType(const std::string& type)
{
	return std::ranges::any_of(this->_supported_types, [type](const std::string& t){return t == type; });
}

std::array<std::string, kSupportedTypesSize> Vector4::getSupporedTypes() const
{
	return this->_supported_types;
}

void Vector4::print(IConsole& console)
{
	std::ostringstream oss;
    oss << "(" << this->_x << ", " << this->_y << ", " << this->_z << ", " << this->_w << ")";
    console.printLine(oss.str());
}

bool Vector4::setData(const std::string& xStr, const std::string& yStr, const std::string& zStr, const std::string& wStr)
{
	try
	{
		if (_type == "int")
		{
			_x = convertTo<int>(xStr);
			_y = convertTo<int>(yStr);
			_z = convertTo<int>(zStr);
			_w = convertTo<int>(wStr);
		}
		else if (_type == "uint")
		{
			_x = convertTo<unsigned int>(xStr);
			_y = convertTo<unsigned int>(yStr);
			_z = convertTo<unsigned int>(zStr);
			_w = convertTo<unsigned int>(wStr);
		}
		else if (_type == "float")
		{
			_x = convertTo<float>(xStr);
			_y = convertTo<float>(yStr);
			_z = convertTo<float>(zStr);
			_w = convertTo<float>(wStr);
		}
		else if (_type == "double")
		{
			_x = convertTo<double>(xStr);
			_y = convertTo<double>(yStr);
			_z = convertTo<double>(zStr);
			_w = convertTo<double>(wStr);
		}
		else if (_type == "char")
		{
			_x = convertTo<char>(xStr);
			_y = convertTo<char>(yStr);
			_z = convertTo<char>(zStr);
			_w = convertTo<char>(wStr);
		}
		else if (_type == "string")
		{
			_x = convertTo<std::string>(xStr);
			_y = convertTo<std::string>(yStr);
			_z = convertTo<std::string>(zStr);
			_w = convertTo<std::string>(wStr);
		}
		else if (_type == "bool")
		{
			_x = convertTo<bool>(xStr);
			_y = convertTo<bool>(yStr);
			_z = convertTo<bool>(zStr);
			_w = convertTo<bool>(wStr);
		}
		else
		{
			std::cout << "Unsupported type: " << _type << "\n";
			return false;
		}
		return true;
	}
	catch (const std::exception& e)
	{
		std::cout << "Conversion error: " << e.what() << "\n";
		return false;
	}
}