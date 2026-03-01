#include "application.h"
#include "data_pool.h"
#include "vector4.h"
#include <sstream>
#include <vector>

MenuItem::MenuItem(command command) : _command(std::move(command)) {};

bool MenuItem::execute(std::vector<std::string>& command_args, Options& opts, DataPool& data, IConsole& console)
{
	this->_command(data, command_args, opts, console);
	return true;
}

Menu::Menu()
{
	this->_items = {
		{"quit", MenuItem(quitProgram)},
		{"help", MenuItem(help)},
		{"username", MenuItem(setUsername)},
		{"type", MenuItem(inputType)},
		{"vec", MenuItem(inputVec)},
		{"add", MenuItem(addVec)},
		{"pop", MenuItem(popVec)},
	};
}

bool Menu::exists(const std::string& command_text)
{
	return this->_items.contains(command_text);
}

bool Menu::execute(const std::string& command_text, std::vector<std::string>& command_args, Options& opts, DataPool& data, IConsole& console)
{
	return this->_items[command_text].execute(command_args, opts, data, console);
}

int runApplication(Options& opts, DataPool& data, IConsole& console) {
	Menu menu = Menu();

	while(!opts.getShouldExit())
	{
		console.print(opts.getUsername() + " > ");

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

		if (!menu.exists(command))
		{
			console.printLine("Unknown command! For list of available commands type 'help'");
			continue;
		}

		menu.execute(command, command_args, opts, data, console);
	}

	return opts.getStatus();
}

void inputType(DataPool& data, std::vector<std::string>& commandArgs, Options&  /*opts*/, IConsole& console)
{
	if (commandArgs.size() <= 1)
	{
		console.printLine("Currently set type: " + data.frontMut().getType()); 
		return;
	}

	const std::string& type = commandArgs[1];

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
	}
}

void inputVec(DataPool& data, std::vector<std::string>& commandArgs, Options&  /*opts*/, IConsole& console)
{
	if (commandArgs.size() == 1)
	{
		data.frontMut().print(console);
		return;
	}

	if (kInputVectorArgc > commandArgs.size())
	{
		console.printLine("Not enough arguments!");
		return;
	}

	if (data.frontMut().setData(commandArgs[1], commandArgs[2], commandArgs[3], commandArgs[4]))
	{
		console.printLine("Vector data updated!");
		data.frontMut().print(console);
	}
}

void addVec(DataPool& data, std::vector<std::string>& commandArgs, Options&  /*opts*/, IConsole& console)
{
	if (kInputVectorArgc > commandArgs.size())
	{
		console.printLine("Not enough arguments!");
		return;
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
		console.printLine("Vector of type " + data.frontMut().getType() + " has been added!");
		data.frontMut().print(console);
	}
}

void popVec(DataPool& data, std::vector<std::string>&  /*commandArgs*/, Options&  /*opts*/, IConsole& console)
{
	if (1 >= data.size())
	{
		console.printLine("Cannot pop the last vector from queue!");
		return;
	}

	console.printLine("Popped vector" + data.frontMut().sprint() + " of type " + data.frontMut().getType());
	data.pop();
}

void help(DataPool&  /*data*/, std::vector<std::string>&  /*commandArgs*/, Options&  /*opts*/, IConsole& console)
{
	clear(console);
	console.printLine("\nAVAILABLE COMMANDS:");
	console.printLine("quit\t\texit the application");
	console.printLine("help\t\tshow this help");
	console.printLine("username [USERNAME]\tset username");
	console.printLine("type [TYPE NAME]\tset type of vector in front of the queue");
	console.printLine("vec X Y Z W\tset values for a 4D vector in front of the queue");
	console.printLine("add [TYPE] X Y Z W\tadd a 4D vector of TYPE to the queue");
	console.printLine("\nSUPPORTED TYPES:");
	console.printLine("\tint, uint\n\tfloat, double\n\tchar, string\n\tbool");
}

void quitProgram(DataPool&  /*data*/, std::vector<std::string>&  /*commandArgs*/, Options&  opts, IConsole& /* console */)
{
	opts.setShouldExit();
}

void setUsername(DataPool&  /*data*/, std::vector<std::string>& commandArgs, Options& opts, IConsole& console)
{
	if (commandArgs.size() <= 1)
	{
		console.printLine("Currently set username: " + opts.getUsername()); 
		return;
	}

	const std::string& role = commandArgs[1];

	opts.setUsername(role);	
}