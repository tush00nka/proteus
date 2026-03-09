#pragma once

#include "console_interface.h"
#include <any>
#include <string>
#include <array>

const size_t kSupportedTypesSize = 7;

/*
 * Class Invariants for Vector4:
 * 
 * 1. _type MUST be one of the strings in _supported_types
 *    - Cannot be empty
 *    - Must match exactly one of: "int", "uint", "float", "double", "char", "string", "bool"
 * 
 * 2. All components (_x, _y, _z, _w) MUST have the same type
 *    - The std::any objects must all hold values of the type specified by _type
 *    - They cannot be empty (should always hold valid values)
 *	  - _w cannot be 0
 * 
 * 3. _supported_types array MUST be fully initialized
 *    - Contains exactly kSupportedTypesSize (7) elements
 *    - All elements must be non-empty strings
 *    - Order doesn't matter, but set must contain all supported types
 * 
 * 4. After construction or setData(), all components are valid
 *    - No component should be an empty std::any
 *    - All components are convertible to the type specified by _type
 * 
 * 5. The convertTo template function must only be called with types
 *    that correspond to strings in _supported_types
 */
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