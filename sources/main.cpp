#include <string>

#include "console_interface.h"
#include "data_pool.h"
#include "options.h"
#include "application.h"

using std::string;


int main(int argc, char ** argv)
{	
	RealConsole console;
	Options opts = Options(argc, argv); 

	DataPool data {};

	return runApplication(opts, data, console);
}