#pragma once

#include "console_interface.h"
#include <any>
#include <string>
#include <array>

const size_t kSupportedTypesSize = 7;

class Vector4 {
private:
	std::array<std::string, kSupportedTypesSize> _supported_types;
	std::string _type;
	std::any _x;
	std::any _y;
	std::any _z;
	std::any _w;

	// helper function to convert to any supported type
	template<typename T>
    T convertTo(const std::string& str) const;

public:
	Vector4();
	explicit Vector4(std::string type);

	bool supportsType(const std::string& type);

	[[nodiscard]] std::array<std::string, kSupportedTypesSize> getSupporedTypes() const;

	std::string getType()
	{
		return this->_type;
	}

	void setType(std::string type)
	{
		this->_type = std::move(type);
		setData("0", "0", "0", "0");
	}

	void print(IConsole& console);
	std::string sprint();
    bool setData(const std::string& xStr, const std::string& yStr, const std::string& zStr, const std::string& wStr);
};