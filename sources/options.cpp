#include <iostream>
#include <memory>
#include <span>
#include "options.h"
#include "address.h"
#include "logger.h"

void Options::usage(std::string_view program_name)
{
	std::cout << "USAGE:\n";
	std::cout << program_name << " [OPTIONS]" << "\n";
	std::cout << "\t" << "-a ADDRESS" << "\n";
	std::cout << "\t" << "-p PORT" << "\n";
	std::cout << "\t" << "-r ROLE" << "\n";
	std::cout << "\t" << "-i INDEX" << "\n";
	std::cout << "\t" << "-L LIB" << "\n";
}

void Options::errorWithMessage(std::string_view program_name, std::string_view message, Logger& logger)
{
	// std::string error_message = "ERROR: Flag '" + arg +"' doesn't seem to have a valid option set!\n";
	log<LogLevel::FATAL>(logger, message);
	std::cout << message << '\n';
	usage(program_name);

	_should_exit = true;
	_status = 1;
}

Options::Options(int argc, char ** argv, Logger& logger) : _address(nullptr), _status(0), _should_exit(false)
{
	for (size_t i = 1; i < argc; ++i)
	{
		auto args = std::span(argv, argc);
		std::string arg = args[i];
		
		if (i+1 >= argc)
		{
			errorWithMessage(args[0], "Flag `"+arg+"` doesn't seem to have a valid option set!", logger);
			break;
		}

		std::string next_arg = args[i+1];
		if(next_arg.starts_with('-'))
		{
			errorWithMessage(args[0], "Flag `"+arg+"` doesn't seem to have a valid option set!", logger);
			break;
		}

		if (arg == "-a")
		{
			std::string address_accum;
			size_t counter = 1;
			std::string current_arg = args[i+counter];
			while(!current_arg.starts_with('-') && counter < args.size()-1)
			{
				current_arg = args[i+counter];
				address_accum += ' ' + current_arg;
				counter++;
			}

			log<LogLevel::DEBUG>(logger, std::format("Got {} Address parts: {}", counter-1, address_accum));

			this->_address = std::make_unique<Address>(Address(address_accum, logger));
			log<LogLevel::INFO>(logger, "Address is set to: "+this->_address->sprint());
			i+=counter-1;
			continue;
		}

		if (arg == "-p")
		{
			if (this->_address == nullptr)
			{
				this->_address = std::make_unique<Address>(Address());
			}

			try
			{
				this->_address->setPort(std::stoi(args[i+1]));
			}
			catch(std::exception& e)
			{
				errorWithMessage(args[0], std::format("Failed to parse `-p` flag argument: {}", e.what()), logger);
				break;
			}	
			log<LogLevel::INFO>(logger, "Address is set to: "+this->_address->sprint()+" (via -p flag)");
			i++;
			continue;
		}

		if (arg == "-r")
		{
			this->_role = args[i+1];
			i++;
			continue;
		}	

		if (arg == "-i")
		{
			try
			{
				this->_index = std::stoi(args[i+1]);
			}
			catch(std::exception& e)
			{
				errorWithMessage(args[0], std::format("Failed to parse `-i` flag argument: {}", e.what()), logger);
				break;
			}	
			i++;
			continue;
		}

		if (arg == "-L")
		{
			this->_lib = args[i+1];
			i++;
			continue;
		}

		std::cout << "ERROR: Unknown option: " << arg << "\n";
		std::string program_name = args[0];
		usage(program_name);
	
		_should_exit = true;
		_status = 1;
		break;
	}

	if (this->_address == nullptr)
	{
		log<LogLevel::WARNING>(logger, "Address (-a) and/or Port (-p) are not set");
	}

	if (this->_role.size() <= 0)
	{
		this->_role = "Client";
	}
}