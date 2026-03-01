#pragma once

#include "console_interface.h"
#include "options.h"
#include "vector4.h"
#include <functional>

const size_t kInputVectorArgc = 5;

using command = std::function<void(Vector4& vec, std::vector<std::string>& commandArgs, Options& opts, IConsole& console)>;

void inputType(Vector4& vec, std::vector<std::string>& commandArgs, Options& opts, IConsole& console);
void inputVec(Vector4& vec, std::vector<std::string>& commandArgs, Options& opts, IConsole& console);
void setUsername(Vector4& vec, std::vector<std::string>& commandArgs, Options& opts, IConsole& console);
void quitProgram(Vector4& vec, std::vector<std::string>& commandArgs, Options& opts, IConsole& console);
void help(Vector4& vec, std::vector<std::string>& commandArgs, Options& opts, IConsole& console);

static void clear(IConsole& console)
{
	console.print("\033[2J\033[1;1H"); // clear the console
}

int runApplication(Options& opts, Vector4& vec, IConsole& console);