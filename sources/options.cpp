#include "options.h"
#include "utils.h"

Options::Options(int argc, char ** argv)
{
	for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];
		if (arg == "-a")
		{
			this->address = argv[i+1];
			i++;
			continue;
		}

		if (arg == "-p")
		{
			this->port = argv[i+1];
			i++;
			continue;
		}

		if (arg == "-r")
		{
			this->role = argv[i+1];
			i++;
			continue;
		}	

		if (arg == "-i")
		{
			this->i = std::stoi(argv[i+1]);
			i++;
			continue;
		}

		if (arg == "-L")
		{
			this->lib = argv[i+1];
			i++;
			continue;
		}

		std::cout << "ERROR: Unknown option: " << arg << "\n";
		std::string programName = argv[0];
		usage(programName);
		exit(-1);
	}

	if (this->role.size() <= 0)
	{
		this->role = "Client";
	}
}