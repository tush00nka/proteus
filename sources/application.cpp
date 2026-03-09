#include "application.h"
#include "accessibility.h"
#include "data_pool.h"
#include "logger.h"
#include "vector4.h"
#include <cstddef>
#include <cstdio>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

MenuItem::MenuItem(command command) : _command(std::move(command)) {};

bool MenuItem::execute(std::vector<std::string>& command_args, Options& opts, DataPool& data, IConsole& console, Logger& logger)
{
	this->_command(data, command_args, opts, console, logger);
	return true;
}

Menu::Menu()
{
	this->_items = {
		{"quit", MenuItem(quitProgram)},
		{"q", MenuItem(quitProgram)},
		{"exit", MenuItem(quitProgram)},
		
		{"help", MenuItem(help)},
		{"username", MenuItem(setUsername)},
		{"type", MenuItem(inputType)},
		{"vec", MenuItem(inputVec)},
		{"add", MenuItem(addVec)},
		{"pop", MenuItem(popVec)},
		{"test", MenuItem(testAccessibility)},
	};
}

bool Menu::exists(const std::string& command_text)
{
	return this->_items.contains(command_text);
}

bool Menu::execute(const std::string& command_text, std::vector<std::string>& command_args, Options& opts, DataPool& data, IConsole& console, Logger& logger)
{
	return this->_items[command_text].execute(command_args, opts, data, console, logger);
}

int runApplication(Options& opts, DataPool& data, IConsole& console, Logger& logger) {
	Menu menu = Menu();
	ResourceTest resource_test = ResourceTest();

	while(!opts.getShouldExit())
	{
		console.print(opts.getUsername() + " > ");

		std::string line = console.readLine();

		log<LogLevel::USER_INPUT>(logger, line);

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

		if (!menu.exists(command))
		{
			console.printLine("Unknown command! For list of available commands type 'help'");
			continue;
		}

		menu.execute(command, command_args, opts, data, console, logger);
	}

	log<LogLevel::INFO>(logger, std::format("App finished with status {}", opts.getStatus()));

	return opts.getStatus();
}

void inputType(DataPool& data, std::vector<std::string>& commandArgs, Options&  /*opts*/, IConsole& console, Logger& logger)
{
	if (commandArgs.size() <= 1)
	{
		console.printLine("Currently set type: " + data.frontMut().getType()); 
		log<LogLevel::INFO>(logger, "Currently set type: " + data.frontMut().getType()); 
		return;
	}

	const std::string& type = commandArgs[1];

	if (data.frontMut().supportsType(type))
	{
		data.frontMut().setType(type);
		console.printLine("Set type: " + type);
		log<LogLevel::INFO>(logger, "Set type: " + type);
	}
	else
	{
		console.printLine("Unknown type! Supported types are:"); 
		log<LogLevel::ERROR>(logger, "Unknown type: " + type);
		for (const auto& t : data.frontMut().getSupporedTypes()) {
			console.printLine("  " + t);
		}
	}
}

void inputVec(DataPool& data, std::vector<std::string>& commandArgs, Options&  /*opts*/, IConsole& console, Logger& logger)
{
	if (commandArgs.size() == 1)
	{
		data.frontMut().print(console);
		return;
	}

	if (kInputVectorArgc < commandArgs.size())
	{
		log<LogLevel::WARNING>(logger, "Too many arguments! Input will be shrunk down to " + std::to_string(kInputVectorArgc-1) + " arguments!");
	}

	if (kInputVectorArgc > commandArgs.size())
	{
		const std::string error_message = "Not enough arguments!";
		console.printLine(error_message);
		log<LogLevel::ERROR>(logger, error_message);
		return;
	}

	if (data.frontMut().setData(commandArgs[1], commandArgs[2], commandArgs[3], commandArgs[4]))
	{
		const std::string message = "Vector data updated!";
		console.printLine(message);
		log<LogLevel::INFO>(logger, message);
		data.frontMut().print(console);
	}
}

void addVec(DataPool& data, std::vector<std::string>& commandArgs, Options&  /*opts*/, IConsole& console, Logger& logger)
{
	if (kInputVectorArgc > commandArgs.size())
	{
		const std::string error_message = "Not enough arguments!";
		console.printLine(error_message);
		log<LogLevel::ERROR>(logger, error_message);
		return;
	}

	if (kAddVectorArgc < commandArgs.size())
	{
		log<LogLevel::WARNING>(logger, "Too many arguments! Input will be shrunk down to " + std::to_string(kInputVectorArgc-1) + " arguments!");
	}

	std::string type;
	size_t offset = 0; 
	if (kAddVectorArgc > commandArgs.size())
	{
		type = data.frontMut().getType();
	}
	else
	{	
		type = commandArgs[1];
		offset = 1;
	}

	data.push(Vector4());

	if (data.frontMut().supportsType(type))
	{
		data.frontMut().setType(type);
		console.printLine("Set type: " + type);
	}
	else
	{
		console.printLine("Unknown type! Supported types are:"); 
		for (const auto& t : data.frontMut().getSupporedTypes()) {
			console.printLine("  " + t);
		}
		return;
	}

	if (data.frontMut().setData(commandArgs[1+offset], commandArgs[2+offset], commandArgs[3+offset], commandArgs[4+offset]))
	{
		const std::string message = "Vector of type " + data.frontMut().getType() + " has been added!";
		console.printLine(message);
		log<LogLevel::INFO>(logger, message);
		data.frontMut().print(console);
	}
}

void popVec(DataPool& data, std::vector<std::string>&  /*commandArgs*/, Options&  /*opts*/, IConsole& console, Logger& logger)
{
	if (1 >= data.size())
	{
		const std::string error_message = "Cannot pop the last vector from queue!"; 
		console.printLine(error_message);
		log<LogLevel::ERROR>(logger, error_message);
		return;
	}

	const std::string message = "Popped vector" + data.frontMut().sprint() + " of type " + data.frontMut().getType(); 
	console.printLine(message);
	log<LogLevel::INFO>(logger, message);
	data.pop();
}

void help(DataPool&  /*data*/, std::vector<std::string>&  /*commandArgs*/, Options&  /*opts*/, IConsole& console, Logger&  /*logger*/)
{
	clear(console);
	console.printLine("\nAVAILABLE COMMANDS:");
	console.printLine("quit\t\texit the application");
	console.printLine("help\t\tshow this help");
	console.printLine("username [USERNAME]\tset username");
	console.printLine("type [TYPE NAME]\tset type of vector in front of the queue");
	console.printLine("vec X Y Z W\tset values for a 4D vector in front of the queue");
	console.printLine("add [TYPE] X Y Z W\tadd a 4D vector of TYPE to the queue");
	console.printLine("test [ADDRESS | PATH] [PORT_1 PORT_2 ... | FILE_1 FILE_2 ...]\t perform ResourceTest or ConnectionTest");
	console.printLine("\nSUPPORTED TYPES:");
	console.printLine("\tint, uint\n\tfloat, double\n\tchar, string\n\tbool");
}

void quitProgram(DataPool&  /*data*/, std::vector<std::string>&  /*commandArgs*/, Options&  opts, IConsole& /* console */, Logger& logger)
{
	log<LogLevel::INFO>(logger, "Asked program to exit");
	opts.setShouldExit();
}

void setUsername(DataPool&  /*data*/, std::vector<std::string>& commandArgs, Options& opts, IConsole& console, Logger& logger)
{
	if (commandArgs.size() <= 1)
	{
		const std::string message = "Currently set username: " + opts.getUsername(); 
		console.printLine(message);
		log<LogLevel::INFO>(logger, message); 
		return;
	}

	const std::string& name = commandArgs[1];

	opts.setUsername(name);
	const std::string message = "Changed username to: " + name; 	
	console.printLine(message);
	log<LogLevel::INFO>(logger, message);
}

void testAccessibility(DataPool & /*data*/, std::vector<std::string> &commandArgs, Options & /*opts*/, IConsole &console, Logger &logger)
{
	if (commandArgs.size() < 3)
	{
		const std::string& message = "Not enough arguments!";
		log<LogLevel::ERROR>(logger, message);
		console.printLine(message);
		return;

	}

	bool connection_test = false;
	if (std::ranges::count(commandArgs[1], '/') <= 0) // if the are no '/', we think of the first argument as an ip-address, otherwise it's a file path
	{
		connection_test = true;
	}

	std::vector<std::string> values = {};

	values.insert(
		values.end(),
		std::make_move_iterator(commandArgs.begin() + 2),
		std::make_move_iterator(commandArgs.end())
	);
	commandArgs.erase(commandArgs.begin() + 2, commandArgs.end());


	if (connection_test)
	{
		ConnectionTest test = ConnectionTest();
		
		std::string result;
		if (test.check(commandArgs[1], values, logger))
		{
			result = "All ports on address are available!";
		}
		else
		{
			result = "Some ports are unavailable!";
		}
		console.printLine(result);
	}
	else
	{
		ResourceTest test = ResourceTest();

		std::string result;
		if (test.check(commandArgs[1], values, logger))
		{
			result = "All files are found!";
		} 
		else {
			result = "Some files are missing!";
		}

		console.printLine("ResourceTest result: " + result);
	}
}