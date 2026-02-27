#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "options.h"
#include "vector4.h"

using std::string;

const size_t kInputVectorArgc = 5;

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

	if (vec.supportsType(type))
	{
		vec.setType(type);
		std::cout << "Set type: " << type << "\n"; 
	}
	else
	{
		std::cout << "Unknown type! Supported types are:\n"; 
		for (const auto& t : vec.getSupporedTypes()) {
			std::cout << "  " << t << "\n";
		}
	}
}

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