#include "application.h"
#include "address.h"
#include "console_interface.h"
#include "logger.h"
#include "ue.h"
#include <exception>
#include <format>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

bool Help::execute(const std::vector<std::string_view>&  /*args*/)
{	
    _console->print("\033[2J\033[1;1H"); // clear the console

	_console->printLine("\n═══════════════════════════════════════════════════════════════════════");
	_console->printLine("  GENERAL COMMANDS");
	_console->printLine("═══════════════════════════════════════════════════════════════════════");
	_console->printLine("  quit                    exit the application");
	_console->printLine("  help                    show this help");
	_console->printLine("═══════════════════════════════════════════════════════════════════════");
	_console->printLine("  move                    set UE position");
	_console->printLine("═══════════════════════════════════════════════════════════════════════");
	return true;
}

bool Menu::exists(std::string_view command_text)
{
	return this->_items.contains(std::string(command_text));
}

bool Menu::execute(std::string_view command_text, const std::vector<std::string_view>& args)
{
	log<LogLevel::INFO>(std::format("Executing command {}...", command_text));
	return this->_items[std::string(command_text)]->execute(args);
}

bool Menu::getShouldExit() const
{
	return *this->_should_exit;
}

void runApplication(const std::shared_ptr<Options>& opts, const std::shared_ptr<IConsole>& console)
{
	Menu menu (console, opts);

	Address addr = opts->getAddress();

	UE ue(*opts);

	ue.connectToServer();
	ue.turnOn();

	while(!menu.getShouldExit())
	{
		console->print(std::format("{} > ", opts->getUsername()));

		std::string line = console->readLine();

		log<LogLevel::USER_INPUT>(line);

		std::vector<std::string_view> command_args;

		std::string_view sv(line);

		command_args = split(sv, ' ');

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

	ue.turnOff();
	ue.disconnectFromServer();
}

bool Quit::execute(const std::vector<std::string_view>&  /*args*/)
{
	*this->_should_exit = true;
	return true;
}

bool Move::execute(const std::vector<std::string_view>& args)
{
	try 
	{
		int new_position = std::stoi(std::string(args.back()));
		this->_options->setPosition(new_position);

		const std::string msg = std::format("Set new position: {}", this->_options->getPosition());
		this->_console->printLine(msg);
		log<LogLevel::INFO>(msg);

		return true;
	}
	catch (std::exception& e)
	{
		const std::string msg = std::format("Failed to set position: {}", e.what());
		this->_console->printLine(msg);
		log<LogLevel::ERROR>(msg);
		return false;
	}
	return true;
}


// bool Send::execute(const std::vector<std::string_view>&  /*args*/)
// {
// 	TCPClient client {};

// 	Address addr = _options->getAddress();
// 	if (!client.connect(addr)) {
// 		const std::string message = "Connection to server failed!";
// 		_console->printLine(message);
// 		log<LogLevel::ERROR>(message);
// 		return false;
// 	}

// 		if (0 >= _data_pool->size())
// 	{
// 		const std::string message = "Empty queue! No vector to send!";
// 		_console->printLine(message);
// 		log<LogLevel::ERROR>(message);
// 		return false;
// 	}

// 	std::string to_send = _data_pool->front().serialize();

// 	if (to_send.empty())
// 	{
// 		const std::string message = "Trying to send empty string!";
// 		_console->printLine(message);
// 		log<LogLevel::ERROR>(message);
// 		return false;
// 	}

// 	std::string response = client.sendAndRecieve(to_send); 

// 	if (_data_pool->front().deserialize(response))
// 	{
// 		_console->printLine("Recieved processed vector: " + _data_pool->front().sprint());
// 		log<LogLevel::INFO>("Recieved processed vector: " + _data_pool->front().sprint());
// 	}
// 	else
// 	{
// 		_console->printLine("Recieved response: " + response);
// 		log<LogLevel::ERROR>("Recieved response: " + response);
// 	}

// 	return true;
// }