#include <iostream>
#include <span>
#include "options.h"
#include "utils.h"

Options::Options(int argc, char ** argv) : _status(0), _should_exit(false)
{
	for (int i = 1; i < argc; ++i)
	{
		auto args = std::span(argv, argc);
		std::string arg = args[i];
		if (arg == "-a")
		{
			this->_address = args[i+1];
			i++;
			continue;
		}

		if (arg == "-p")
		{
			this->_port = args[i+1];
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
			this->_index = std::stoi(args[i+1]);
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
	}

	if (this->_role.size() <= 0)
	{
		this->_role = "Client";
	}
}