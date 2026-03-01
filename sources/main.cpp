#include <string>

#include "console_interface.h"
#include "options.h"
#include "vector4.h"
#include "application.h"

using std::string;


int main(int argc, char ** argv)
{	
	RealConsole console;
	Options opts = Options(argc, argv); 
	Vector4 vec {};

	return runApplication(opts, vec, console);
}