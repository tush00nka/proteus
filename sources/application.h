#pragma once

#include "console_interface.h"
#include "data_pool.h"
#include "options.h"
#include <functional>

const size_t kInputVectorArgc = 5;
const size_t kAddVectorArgc = 6;

using command = std::function<void(DataPool& data, std::vector<std::string>& commandArgs, Options& opts, IConsole& console)>;

void inputType(DataPool& data, std::vector<std::string>& commandArgs, Options& opts, IConsole& console);
void inputVec(DataPool& data, std::vector<std::string>& commandArgs, Options& opts, IConsole& console);
void addVec(DataPool& data, std::vector<std::string>& commandArgs, Options& opts, IConsole& console);
void setUsername(DataPool& data, std::vector<std::string>& commandArgs, Options& opts, IConsole& console);
void quitProgram(DataPool& data, std::vector<std::string>& commandArgs, Options& opts, IConsole& console);
void help(DataPool& data, std::vector<std::string>& commandArgs, Options& opts, IConsole& console);

void popVec(DataPool& data, std::vector<std::string>& commandArgs, Options& opts, IConsole& console);

static void clear(IConsole& console)
{
	console.print("\033[2J\033[1;1H"); // clear the console
}

int runApplication(Options& opts, DataPool& data, IConsole& console);