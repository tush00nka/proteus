#include "application.h"
#include "accessibility.h"
#include "address.h"
#include "client.h"
#include "console_interface.h"
#include "data_pool.h"
#include "logger.h"
#include "vector4.h"
#include <cstddef>
#include <cstdio>
#include <memory>
#include <ranges>
#include <string>
#include <vector>
#include <algorithm>

bool Help::execute(const std::vector<std::string_view>&  /*args*/)
{	
	// IConsole& console = *_console;
    _console->print("\033[2J\033[1;1H"); // clear the console

	_console->printLine("\n═══════════════════════════════════════════════════════════════════════");
	_console->printLine("  GENERAL COMMANDS");
	_console->printLine("═══════════════════════════════════════════════════════════════════════");
	_console->printLine("  quit                    exit the application");
	_console->printLine("  help                    show this help");
	_console->printLine("  username [USERNAME]     set username");

	_console->printLine("\n═══════════════════════════════════════════════════════════════════════");
	_console->printLine("  VECTOR OPERATIONS");
	_console->printLine("═══════════════════════════════════════════════════════════════════════");
	_console->printLine("  type [TYPE NAME]        set type of vector in front of the queue");
	_console->printLine("  vec X Y Z W             set values for a 4D vector in front of the queue");
	_console->printLine("  add [TYPE] X Y Z W      add a 4D vector of TYPE to the queue");
	_console->printLine("  send                    send vector to server for processing");

	_console->printLine("\n═══════════════════════════════════════════════════════════════════════");
	_console->printLine("  TESTING");
	_console->printLine("═══════════════════════════════════════════════════════════════════════");
	_console->printLine("  test [ADDRESS|PATH] [PORT...|FILE...]");
	_console->printLine("                          perform ResourceTest or ConnectionTest");

	_console->printLine("\n═══════════════════════════════════════════════════════════════════════");
	_console->printLine("  SUPPORTED TYPES: int, uint, float, double, char, string, bool");
	_console->printLine("═══════════════════════════════════════════════════════════════════════");
	return true;
}

bool Quit::execute(const std::vector<std::string_view>&  /*args*/) {
	_options->setShouldExit();
	return true;
}

bool SetUsername::execute(const std::vector<std::string_view>&  args) {
	if (args.size() <= 1)
	{
		const std::string message = std::format("Currently set username: {}", _options->getUsername()); 
		_console->printLine(message);
		log<LogLevel::INFO>(message); 
		return true;
	}

	std::string_view name = args[1];
	_options->setUsername(name);
	
	const std::string message = std::format("Changed username to: {}", name); 	
	_console->printLine(message);
	log<LogLevel::INFO>(message);

	return true;
}

// Menu::Menu(std::shared_ptr<IConsole> console)
// {
// 	this->_items = {
// 		// {"test", MenuItem(testAccessibility)},
// 	};
// }

bool Menu::exists(std::string_view command_text)
{
	return this->_items.contains(std::string(command_text));
}

bool Menu::execute(std::string_view command_text, const std::vector<std::string_view>& args)
{
	log<LogLevel::INFO>(std::format("Executing command {}...", command_text));
	return this->_items[std::string(command_text)]->execute(args);
}

int runApplication(const std::shared_ptr<Options>& opts, const std::shared_ptr<IConsole>& console)
{
	std::shared_ptr<DataPool> data = std::make_shared<DataPool>();

	Menu menu (data, console, opts);

	Address addr = opts->getAddress();

	while(!opts->getShouldExit())
	{
		console->print(std::format("{} > ", opts->getUsername()));

		std::string line = console->readLine();

		log<LogLevel::USER_INPUT>(line);

		std::vector<std::string_view> command_args;

		std::string_view sv(line);

		command_args = sv | std::views::split(' ')
						| std::views::transform([](auto&& r) {
							return std::string_view(r.begin(), r.end());
						})
						| std::ranges::to<std::vector<std::string_view>>();

		if (command_args.empty())
		{
			continue;
		}

		for (auto arg : command_args)
		{
			log<LogLevel::DEBUG>(arg);
		}
		
		std::string command{command_args[0].data(), command_args[0].length()};

		if (!menu.exists(command))
		{
			console->printLine("Unknown command! For list of available commands type 'help'");
			continue;
		}

		if (!menu.execute(command, command_args)) {
			log<LogLevel::ERROR>(std::format("Command '{}' executed with error", command));
			console->printLine(std::format("Command '{}' executed with error", command));
			continue;
		}
	}

	log<LogLevel::INFO>(std::format("App finished with status {}", opts->getStatus()));

	return opts->getStatus();
}

bool InputType::execute(const std::vector<std::string_view>& args)
{
	if (0 >= _data_pool->size()) {
		const std::string message = "Empty queue! No vector to work with!";
		_console->printLine(message);
		log<LogLevel::ERROR>(message);
		return false;
	}
	
	if (args.size() <= 1)
	{
		_console->printLine("Currently set type: " + _data_pool->front().getType()); 
		log<LogLevel::INFO>("Currently set type: " + _data_pool->front().getType()); 
		return false;
	}

	const std::string_view& type = args[1];

	if (_data_pool->front().setType(type))
	{
		_console->printLine(std::format("Set type: {}", type));
		log<LogLevel::INFO>(std::format("Set type: {}", type));
	}
	else
	{
		_console->printLine("Unknown type! Supported types are:"); 
		log<LogLevel::ERROR>(std::format("Unknown type: {}", type));
		for (const auto& t : _data_pool->front().getSupporedTypes()) {
			_console->printLine("  " + t);
		}
	}

	return true;
}

bool InputVec::execute(const std::vector<std::string_view>& commandArgs)
{
	if (0 >= _data_pool->size()) {
		const std::string message = "Empty queue! No vector to work with!";
		_console->printLine(message);
		log<LogLevel::ERROR>(message);
		return false;
	}

	if (commandArgs.size() == 1)
	{
		_console->printLine(_data_pool->front().sprint());
		return true;
	}

	if (kInputVectorArgc < commandArgs.size())
	{
		log<LogLevel::WARNING>("Too many arguments! Input will be shrunk down to " + std::to_string(kInputVectorArgc-1) + " arguments!");
	}

	if (kInputVectorArgc > commandArgs.size())
	{
		const std::string error_message = "Not enough arguments!";
		_console->printLine(error_message);
		log<LogLevel::ERROR>(error_message);
		return false;
	}

	if (_data_pool->front().setData(commandArgs[1], commandArgs[2], commandArgs[3], commandArgs[4]))
	{
		const std::string message = "Vector data updated!";
		_console->printLine(message);
		log<LogLevel::INFO>(message);
		_console->printLine(_data_pool->front().sprint());

		return true;
	}

	return false;
}

bool AddVec::execute(const std::vector<std::string_view>& commandArgs)
{
	if (kInputVectorArgc > commandArgs.size())
	{
		const std::string error_message = "Not enough arguments!";
		_console->printLine(error_message);
		log<LogLevel::ERROR>(error_message);
		return false;
	}

	if (kAddVectorArgc < commandArgs.size())
	{
		log<LogLevel::WARNING>("Too many arguments! Input will be shrunk down to " + std::to_string(kInputVectorArgc-1) + " arguments!");
	}

	std::string type;
	size_t offset = 0; 
	if (kAddVectorArgc > commandArgs.size())
	{
		type = _data_pool->front().getType();
	}
	else
	{	
		type = commandArgs[1];
		offset = 1;
	}

	_data_pool->push(Vector4());

	if (_data_pool->front().setType(type))
	{
		_console->printLine("Set type: " + type);
	}
	else
	{
		_console->printLine("Unknown type! Supported types are:"); 
		for (const auto& t : _data_pool->front().getSupporedTypes()) {
			_console->printLine("  " + t);
		}
		return false;
	}

	if (_data_pool->front().setData(commandArgs[1+offset], commandArgs[2+offset], commandArgs[3+offset], commandArgs[4+offset]))
	{
		const std::string message = "Vector of type " + _data_pool->front().getType() + " has been added!";
		_console->printLine(message);
		log<LogLevel::INFO>(message);
		_console->printLine(_data_pool->front().sprint());
	
		return true;
	}

	return false;
}

bool PopVec::execute(const std::vector<std::string_view>& /*args*/)
{
	if (0 >= _data_pool->size())
	{
		const std::string error_message = "Nothing to pop!"; 
		_console->printLine(error_message);
		log<LogLevel::ERROR>(error_message);
		return false;
	}

	const std::string message = "Popped vector" + _data_pool->front().sprint() + " of type " + _data_pool->front().getType(); 
	_console->printLine(message);
	log<LogLevel::INFO>(message);
	_data_pool->pop();

	return true;
}

bool TestAccessibility::execute(const std::vector<std::string_view>& args)
{
	if (args.size() < 3)
	{
		const std::string& message = "Not enough arguments!";
		log<LogLevel::ERROR>(message);
		_console->printLine(message);
		return false;

	}

	bool connection_test = false;
	if (std::ranges::count(args[1], '/') <= 0) // if the are no '/', we think of the first argument as an ip-address, otherwise it's a file path
	{
		connection_test = true;
	}

	std::vector<std::string> values = {};

	values.insert(
		values.end(),
		std::make_move_iterator(args.begin() + 2),
		std::make_move_iterator(args.end())
	);
	// args.erase(args.begin() + 2, args.end());


	if (connection_test)
	{
		ConnectionTest test = ConnectionTest();
		
		std::string result;
		if (test.check(args[1], values))
		{
			result = "There are available ports!";
		}
		else
		{
			result = "All ports are unavailable :(";
		}
		_console->printLine(result);
	}
	else
	{
		ResourceTest test = ResourceTest();

		std::string result;
		if (test.check(args[1], values))
		{
			result = "All files are found!";
		} 
		else {
			result = "Some files are missing!";
		}

		_console->printLine("ResourceTest result: " + result);
	}

	return true;
}

bool Send::execute(const std::vector<std::string_view>&  /*args*/)
{
	TCPClient client {};

	Address addr = _options->getAddress();
	if (!client.connect(addr)) {
		const std::string message = "Connection to server failed!";
		_console->printLine(message);
		log<LogLevel::ERROR>(message);
		return false;
	}

		if (0 >= _data_pool->size())
	{
		const std::string message = "Empty queue! No vector to send!";
		_console->printLine(message);
		log<LogLevel::ERROR>(message);
		return false;
	}

	std::string to_send = _data_pool->front().serialize();

	if (to_send.empty())
	{
		const std::string message = "Trying to send empty string!";
		_console->printLine(message);
		log<LogLevel::ERROR>(message);
		return false;
	}

	std::string response = client.sendAndRecieve(to_send); 

	if (_data_pool->front().deserialize(response))
	{
		_console->printLine("Recieved processed vector: " + _data_pool->front().sprint());
		log<LogLevel::INFO>("Recieved processed vector: " + _data_pool->front().sprint());
	}
	else
	{
		_console->printLine("Failed to deserialize server response");
		log<LogLevel::ERROR>("Failed to deserialize server response: " + response);
	}

	return true;
}