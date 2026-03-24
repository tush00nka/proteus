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
	std::shared_ptr<Options> opts = std::make_shared<Options>(argc, argv); 

	Logger::shutdown();

	return runApplication(opts, console);
}