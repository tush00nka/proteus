#include <memory>
#include <string>

#include "console_interface.h"
#include "logger.h"
#include "options.h"
#include "application.h"

using std::string;


int main(int argc, char ** argv)
{	
	Logger::init("proteus.log", true);
	Logger::clear();

	log<LogLevel::INFO>("App started");

	std::shared_ptr<RealConsole> console = std::make_shared<RealConsole>();
	std::shared_ptr<Options> opts = std::make_shared<Options>();
	
	int status = 0;
	if (!opts->parseArguments(argc, argv)) 
	{
		const std::string_view msg = "Failed to parse command line arguments. Exiting...";
		console->printLine(std::string(msg));
		log<LogLevel::ERROR>(msg);
		status = -1;
	}
	else 
	{
		runApplication(opts, console);
	}

	Logger::shutdown();

	return status;
}