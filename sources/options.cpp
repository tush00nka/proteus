#include <cstdio>
#include <iostream>
#include <optional>
#include <print>
#include <span>
#include "options.h"
#include "address.h"
#include "logger.h"

void Options::usage(std::string_view program_name)
{
	std::println("USAGE:");
	std::println("{} [OPTIONS]", program_name);
	std::println("\t-a ADDRESS");
	std::println("\t-p PORT");
	std::println("\t-r ROLE");
	std::println("\t-i INDEX");
	std::println("\t-L LIB");
}

void Options::errorWithMessage(std::string_view program_name, std::string_view message)
{
	log<LogLevel::FATAL>(message);
	std::cout << message << '\n';
	usage(program_name);

	_should_exit = true;
	_status = 1;
}

Options::Options(int argc, char ** argv) : _address(std::nullopt), _status(0), _should_exit(false)
{
	for (size_t i = 1; i < argc; ++i)
	{
		auto args = std::span(argv, argc);
		std::string arg = args[i];
		
		if (i+1 >= argc)
		{
			errorWithMessage(args[0], "Flag `"+arg+"` doesn't seem to have a valid option set!");
			break;
		}

		std::string next_arg = args[i+1];
		if(next_arg.starts_with('-'))
		{
			errorWithMessage(args[0], "Flag `"+arg+"` doesn't seem to have a valid option set!");
			break;
		}

		if (arg == "-a")
		{
			std::string address_accum;
			size_t counter = 1;
			std::string current_arg = args[i+counter];
			while(counter < args.size()-1)
			{
				current_arg = args[i+counter];

				if (current_arg.starts_with('-'))
				{
					break;
				}

				address_accum += ' ' + current_arg;
				counter++;
			}

			log<LogLevel::DEBUG>(std::format("Got {} Address parts: {}", counter-1, address_accum));

			this->_address = (Address(address_accum));
			log<LogLevel::INFO>("Address is set to: "+this->_address->sprint());
			i+=counter-1;

			continue;
		}

		if (arg == "-p")
		{
			if (this->_address == std::nullopt)
			{
				this->_address = Address();
			}

			try
			{
				this->_address->setPort(std::stoi(args[i+1]));
			}
			catch(std::exception& e)
			{
				errorWithMessage(args[0], std::format("Failed to parse `-p` flag argument: {}", e.what()));
				break;
			}	

			log<LogLevel::INFO>("Address is set to: "+this->_address->sprint()+" (via -p flag)");
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
				errorWithMessage(args[0], std::format("Failed to parse `-i` flag argument: {}", e.what()));
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

	if (this->_address == std::nullopt)
	{
		log<LogLevel::WARNING>("Address (-a) and/or Port (-p) are not set");
		this->_address = Address();
	}

	if (this->_role.size() <= 0)
	{
		this->_role = "Client";
	}
}

Address Options::getAddress() const
{
	return *this->_address;
}