#include <iostream>
#include <string>

#include "options.h"

int main(int argc, char ** argv)
{	
	Options opts = Options(argc, argv); 

	while(1)
	{
		std::string command;
		std::cin >> command;
	
		if (command == "quit")
		{
			break;
		}
	}

	return 0;
}