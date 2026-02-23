#include <iostream>
#include <string>

int main(int argc, char ** argv)
{
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