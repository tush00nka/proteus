#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "console_interface.h"
#include "options.h"
#include "vector4.h"

using std::string;

const size_t kInputVectorArgc = 5;

using command = std::function<void(Vector4& vec, std::vector<std::string>& commandArgs, Options& opts, IConsole& console)>;

static void inputType(Vector4& vec, std::vector<std::string>& commandArgs, Options& opts, IConsole& console);
static void inputVec(Vector4& vec, std::vector<std::string>& commandArgs, Options& opts, IConsole& console);
static void setRole(Vector4& vec, std::vector<std::string>& commandArgs, Options& opts, IConsole& console);
static void quitProgram(Vector4& vec, std::vector<std::string>& commandArgs, Options& opts, IConsole& console);
static void help(Vector4& vec, std::vector<std::string>& commandArgs, Options& opts, IConsole& console);

static void clear(IConsole& console)
{
	console.print("\033[2J\033[1;1H"); // clear the console
}

static int runApplication(Options& opts, Vector4& vec, IConsole& console) {
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

		std::string line = console.readLine();
		
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
			console.printLine("Unknown command! For list of available commands type 'help'");
			continue;
		}

		commands[command](vec, command_args, opts, console);
	}

	return opts.getStatus();
}

int main(int argc, char ** argv)
{	
	RealConsole console;
	Options opts = Options(argc, argv); 
	Vector4 vec {};

	return runApplication(opts, vec, console);
}


void inputType(Vector4& vec, std::vector<std::string>& commandArgs, Options&  /*opts*/, IConsole& console)
{
	if (commandArgs.size() <= 1)
	{
		console.printLine("Currently set type: " + vec.getType()); 
		return;
	}

	const std::string& type = commandArgs[1];

	if (vec.supportsType(type))
	{
		vec.setType(type);
		console.printLine("Set type: " + type);
	}
	else
	{
		console.printLine("Unknown type! Supported types are:"); 
		for (const auto& t : vec.getSupporedTypes()) {
			console.printLine("  " + t);
		}
	}
}

void inputVec(Vector4& vec, std::vector<std::string>& commandArgs, Options&  /*opts*/, IConsole& console)
{
	if (commandArgs.size() == 1)
	{
		vec.print(console);
		return;
	}

	if (kInputVectorArgc > commandArgs.size()) {
		console.printLine("Not enough arguments!");
		return;
	}

        if (vec.setData(commandArgs[1], commandArgs[2], commandArgs[3], commandArgs[4]))
	{
		console.printLine("Vector data updated!");
		vec.print(console);
	}
}

void help(Vector4&  /*vec*/, std::vector<std::string>&  /*commandArgs*/, Options&  /*opts*/, IConsole& console)
{
	clear(console);
	console.printLine("\nAVAILABLE COMMANDS:");
	console.printLine("quit\t\texit the application");
	console.printLine("help\t\tshow this help");
	console.printLine("role [ROLE NAME]\tset role for user");
	console.printLine("type [TYPE NAME]\tset type of vector");
	console.printLine("vec X Y Z W\tset values for a 4D vector");
	console.printLine("\nSUPPORTED TYPES:");
	console.printLine("\tint, uint\n\tfloat, double\n\tchar, string\n\tbool");
}

void quitProgram(Vector4&  /*vec*/, std::vector<std::string>&  /*commandArgs*/, Options&  opts, IConsole& /* console */)
{
	opts.setShouldExit();
}

void setRole(Vector4&  /*vec*/, std::vector<std::string>& commandArgs, Options& opts, IConsole& console)
{
	if (commandArgs.size() <= 1)
	{
		console.printLine("Currently set role: " + opts.getRole()); 
		return;
	}

	const std::string& role = commandArgs[1];

	opts.setRole(role);	
}