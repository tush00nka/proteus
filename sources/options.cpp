#include <cstdio>
#include <exception>
#include <iostream>
#include <print>
#include <span>
#include <string>
#include "options.h"
#include "address.h"
#include "logger.h"

void Options::usage(std::string_view program_name)
{
	std::println("USAGE:");
	std::println("{} [OPTIONS]", program_name);
	std::println("\t-a ADDRESS");
	std::println("\t-p PORT");
	std::println("\t-x POSITION");
	std::println("\t--msisdn MSISDN");
	std::println("\t--imei IMEI");
	std::println("\t--imsi IMSI");
}

void Options::errorWithMessage(std::string_view program_name, std::string_view message)
{
	log<LogLevel::FATAL>(message);
	std::cout << message << '\n';
	usage(program_name);
}

bool Options::parseArguments(int argc, char ** argv)
{
	bool got_address = false;
	bool got_port = false;

	for (size_t i = 1; i < argc; ++i)
	{
		auto args = std::span(argv, argc);
		std::string arg = args[i];
		
		if (i+1 >= argc)
		{
			errorWithMessage(args[0], "Flag `"+arg+"` doesn't seem to have a valid option set!");
			return false;
		}

		std::string next_arg = args[i+1];
		if(next_arg.starts_with('-'))
		{
			errorWithMessage(args[0], "Flag `"+arg+"` doesn't seem to have a valid option set!");
			return false;
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
			log<LogLevel::INFO>("Address is set to: "+this->_address.sprint());
			i+=counter-1;

			got_address = true;

			continue;
		}

		if (arg == "-p")
		{
			try
			{
				this->_address.setPort(std::stoi(args[i+1]));
			}
			catch(std::exception& e)
			{
				errorWithMessage(args[0], std::format("Failed to parse `-p` flag argument: {}", e.what()));
				return false;
			}	

			log<LogLevel::INFO>("Address is set to: "+this->_address.sprint()+" (via -p flag)");
			i++;
			
			got_port = true;

			continue;
		}

		if (arg == "-x")
		{
			try
			{
				this->_position = std::stoi(args[i+1]);
			}
			catch(std::exception& e)
			{
				errorWithMessage(args[0], std::format("Failed to parse `-x` flag argument: {}", e.what()));
				return false;
			}	

			log<LogLevel::INFO>(std::format("Position is set to: {}", this->_position));
			i++;
			
			continue;
		}

		if (arg == "--msisdn")
		{
			if (std::string(args[i+1]).starts_with('-')) {
				errorWithMessage(args[0], "Argument for flag `--imei` was not provided");
				return false;
			}

			try
			{
				this->_msisdn = std::stoull(args[i+1]);
				log<LogLevel::INFO>(std::format("MSISDN is set to: {}", this->_msisdn));
			} 
			catch (std::exception& e)
			{
				errorWithMessage(args[0], std::format("Failed to parse `--msisdn` flag argument: {}", e.what()));
				return false;
			}
			i++;
			continue;
		}


		if (arg == "--imei")
		{
			if (std::string(args[i+1]).starts_with('-')) {
				errorWithMessage(args[0], "Argument for flag `--imei` was not provided");
				return false;
			}

			try
			{
				this->_imei = std::stoull(args[i+1]);
				log<LogLevel::INFO>(std::format("IMEI is set to: {}", this->_imei));
			} 
			catch (std::exception& e)
			{
				errorWithMessage(args[0], std::format("Failed to parse `--imei` flag argument: {}", e.what()));
				return false;
			}
			i++;
			continue;
		}


		if (arg == "--imsi")
		{
			if (std::string(args[i+1]).starts_with('-')) {
				errorWithMessage(args[0], "Argument for flag `--imei` was not provided");
				return false;
			}

			try
			{
				this->_imsi = std::stoull(args[i+1]);
				log<LogLevel::INFO>(std::format("IMSI is set to: {}", this->_imsi));
			} 
			catch (std::exception& e)
			{
				errorWithMessage(args[0], std::format("Failed to parse `--imsi` flag argument: {}", e.what()));
				return false;
			}
			i++;
			continue;
		}

		std::cout << "ERROR: Unknown option: " << arg << "\n";
		std::string program_name = args[0];
		usage(program_name);
	
		return false;
	}

	if (!got_address || !got_port)
	{
		log<LogLevel::ERROR>("Address (-a) and/or Port (-p) are not set");
		return false;
	}

	return true;
}

Address Options::getAddress() const
{
	return this->_address;
}

bool Options::setPosition(int newPosition) 
{
	this->_position = newPosition;
	return true;
}