#include <any>
#include <array>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "options.h"
#include "utils.h"

using std::string;

class Vector4 {
private:
	std::string _type;
	std::any _x;
	std::any _y;
	std::any _z;
	std::any _w;

	// helper function to convert to any supported type
	template<typename T>
    T convertTo(const std::string& str) const {
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
public:
	Vector4(): _type("int"), _x(0), _y(0), _z(0), _w(0) {};

	std::string getType()
	{
		return this->_type;
	}
	void setType(std::string type)
	{
		this->_type = std::move(type);
		setData("0", "0", "0", "0");
	}

	void print()
	{
		std::cout << "(" << this->_x << ", " << this->_y << ", " << this->_z << ", " << this->_w << ")\n";
	}

    bool setData(const std::string& xStr, const std::string& yStr, const std::string& zStr, const std::string& wStr)
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
};

using command = std::function<void(Vector4& vec, std::vector<std::string>& commandArgs, Options& opts)>;

static void inputType(Vector4& vec, std::vector<std::string>& commandArgs, Options& opts);
static void inputVec(Vector4& vec, std::vector<std::string>& commandArgs, Options& opts);
static void setRole(Vector4& vec, std::vector<std::string>& commandArgs, Options& opts);
static void quitProgram(Vector4& vec, std::vector<std::string>& commandArgs, Options& opts);
static void help(Vector4& vec, std::vector<std::string>& commandArgs, Options& opts);

static void clear()
{
	std::cout << "\033[2J\033[1;1H"; // clear the console
}

enum class SupportedTypes: std::uint8_t {
	Int,
	Uint,
	Float,
	Double,
	Char,
	String,
	Bool	
};

const std::array<std::string, 7> kSupportedTypes = {"int",  "uint",   "float", "double",
                                  "char", "string", "bool"};

int main(int argc, char ** argv)
{	
	Options opts = Options(argc, argv); 
	Vector4 vec {};

	std::unordered_map<std::string, command> commands = {
		{"quit", quitProgram},
		{"help", help},
		{"role", setRole},
		{"type", inputType},
		{"vec", inputVec},
	};


	while(!opts.getShouldExit())
	{
		std::cout << opts.getRole() <<" > ";

		std::string line;
		std::getline(std::cin, line);
		
		std::vector<std::string> command_args;
		std::string arg;
		std::istringstream iss(line); 
		while (iss >> arg)
		{
			command_args.push_back(arg);
		}

		if (command_args.empty())
		{
			continue;
		}

		std::string command = command_args[0];

		if (!commands.contains(command))
		{
			std::cout << "Unknown command! For list of available commands type 'help'\n";
			continue;
		}

		commands[command](vec, command_args, opts);
	}

	return opts.getStatus();
}


void inputType(Vector4& vec, std::vector<std::string>& commandArgs, Options&  /*opts*/)
{
	if (commandArgs.size() <= 1)
	{
		std::cout << "Currently set type: " << vec.getType() << "\n"; 
		return;
	}

	const std::string& type = commandArgs[1];

	bool supported = false;
	for (const std::string& t : kSupportedTypes)
	{
		if (t == type)
		{
			supported = true;
			break;
		}
	}

	if (supported)
	{
		vec.setType(type);
		std::cout << "Set type: " << type << "\n"; 
	}
	else
	{
		std::cout << "Unknown type! Supported types are:\n"; 
		for (const auto& t : kSupportedTypes) {
			std::cout << "  " << t << "\n";
		}
	}
}

const size_t kInputVectorArgc = 5; 

void inputVec(Vector4& vec, std::vector<std::string>& commandArgs, Options&  /*opts*/)
{
	if (commandArgs.size() == 1)
	{
		vec.print();
		return;
	}

	if (kInputVectorArgc > commandArgs.size()) {
		std::cout << "Not enough arguments!\n";
		return;
	}

        if (vec.setData(commandArgs[1], commandArgs[2], commandArgs[3], commandArgs[4]))
	{
		std::cout << "Vector data updated!\n";
		vec.print();
	}
}

void help(Vector4&  /*vec*/, std::vector<std::string>&  /*commandArgs*/, Options&  /*opts*/)
{
	clear();
	std::cout << "\nAVAILABLE COMMANDS:\n";
	std::cout << "quit\t\texit the application\n";
	std::cout << "help\t\tshow this help\n";
	std::cout << "role [ROLE NAME]\tset role for user\n";
	std::cout << "type [TYPE NAME]\tset type of vector\n";
	std::cout << "vec X Y Z W\tset values for a 4D vector\n";
	std::cout << "\nSUPPORTED TYPES:\n";
	std::cout << "\tint, uint\n\tfloat, double\n\tchar, string\n\tbool\n";
}

void quitProgram(Vector4&  /*vec*/, std::vector<std::string>&  /*commandArgs*/, Options&  opts)
{
	opts.setShouldExit();
}

void setRole(Vector4&  /*vec*/, std::vector<std::string>& commandArgs, Options& opts)
{
	if (commandArgs.size() <= 1)
	{
		std::cout << "Currently set role: " << opts.getRole() << "\n"; 
		return;
	}

	const std::string& role = commandArgs[1];

	opts.setRole(role);	
}