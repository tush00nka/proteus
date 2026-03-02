#include <string>

#include "console_interface.h"
#include "data_pool.h"
#include "logger.h"
#include "options.h"
#include "application.h"

using std::string;


int main(int argc, char ** argv)
{	
	Logger logger("proteus.log", true);

	logger.clear();

	LOG_INFO(logger, "App started");

	RealConsole console;
	Options opts = Options(argc, argv, logger); 

	DataPool data {};

	return runApplication(opts, data, console, logger);
}