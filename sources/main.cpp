#include <any>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "options.h"
#include "utils.h"

class Vector4 {
private:
	std::string type;
	std::any x;
	std::any y;
	std::any z;
	std::any w;

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
            if (str == "true" || str == "1" || str == "yes") return true;
            return false;
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            return str;
        }
        else {
            return T{};
        }
    }
public:
	Vector4(): type("int"), x(0), y(0), z(0), w(0) {};

	std::string GetType()
	{
		return this->type;
	}
	void SetType(std::string type)
	{
		this->type = type;
		SetData("0", "0", "0", "0");
	}

	void print()
	{
		std::cout << "(" << this->x << ", " << this->y << ", " << this->z << ", " << this->w << ")\n";
	}

    bool SetData(const std::string& xStr, const std::string& yStr, const std::string& zStr, const std::string& wStr)
	{
        try
		{
            if (type == "int")
			{
                x = convertTo<int>(xStr);
                y = convertTo<int>(yStr);
                z = convertTo<int>(zStr);
                w = convertTo<int>(wStr);
            }
            else if (type == "uint")
			{
                x = convertTo<unsigned int>(xStr);
                y = convertTo<unsigned int>(yStr);
                z = convertTo<unsigned int>(zStr);
                w = convertTo<unsigned int>(wStr);
            }
            else if (type == "float")
			{
                x = convertTo<float>(xStr);
                y = convertTo<float>(yStr);
                z = convertTo<float>(zStr);
                w = convertTo<float>(wStr);
            }
            else if (type == "double")
			{
                x = convertTo<double>(xStr);
                y = convertTo<double>(yStr);
                z = convertTo<double>(zStr);
                w = convertTo<double>(wStr);
            }
            else if (type == "char")
			{
                x = convertTo<char>(xStr);
                y = convertTo<char>(yStr);
                z = convertTo<char>(zStr);
                w = convertTo<char>(wStr);
            }
            else if (type == "string")
			{
                x = convertTo<std::string>(xStr);
                y = convertTo<std::string>(yStr);
                z = convertTo<std::string>(zStr);
                w = convertTo<std::string>(wStr);
            }
            else if (type == "bool")
			{
                x = convertTo<bool>(xStr);
                y = convertTo<bool>(yStr);
                z = convertTo<bool>(zStr);
                w = convertTo<bool>(wStr);
            }
            else
			{
                std::cout << "Unsupported type: " << type << "\n";
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

void clear()
{
	std::cout << "\033[2J\033[1;1H"; // clear the console
}

void help()
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

int main(int argc, char ** argv)
{	
	Options opts = Options(argc, argv); 
	Vector4 vec {};
	std::string supportedTypes[] = {"int", "uint",
								    "float", "double",
									"char", "string",
									"bool"};

	while(1)
	{
		std::cout << opts.GetRole() <<" > ";

		std::string line;
		std::getline(std::cin, line);
		
		std::vector<std::string> commandArgs;
		std::string arg;
		std::istringstream iss(line); 
		while (iss >> arg)
		{
			commandArgs.push_back(arg);
		}

		if (commandArgs.empty())
		{
			continue;
		}

		std::string command = commandArgs[0];

		if (command == "quit" || command == "q")
		{
			break;
		}

		if (command == "help")
		{
			help();
			continue;
		}

		if (command == "role")
		{
			if (commandArgs.size() <= 1)
			{
				std::cout << "Currently set role: " << opts.GetRole() << "\n"; 
				continue;
			}

			std::string role = commandArgs[1];

			opts.SetRole(role);
			continue;
		}

		if (command == "type")
		{
			if (commandArgs.size() <= 1)
			{
				std::cout << "Currently set type: " << vec.GetType() << "\n"; 
				continue;
			}

			std::string type = commandArgs[1];

			bool supported = false;
			for (std::string t : supportedTypes)
			{
				if (t == type)
				{
					supported = true;
					break;
				}
			}

			if (supported)
			{
				vec.SetType(type);
				std::cout << "Set type: " << type << "\n"; 
			}
			else
			{
				std::cout << "Unknown type! Supported types are:\n"; 
				for (const auto& t : supportedTypes) {
                    std::cout << "  " << t << "\n";
                }
			}

			continue;
		}

		if (command == "vec")
		{
			if (commandArgs.size() == 1)
			{
				vec.print();
				continue;
			}

			if (commandArgs.size() < 5)
			{
				std::cout << "Not enough arguments!\n";
				continue; 
			}

			if (vec.SetData(commandArgs[1], commandArgs[2], commandArgs[3], commandArgs[4]))
			{
				std::cout << "Vector data updated!\n";
				vec.print();
			}

			continue;
		}

		std::cout << "Unknown command! For list of available commands type 'help'\n";
	}

	return 0;
}