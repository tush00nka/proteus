#pragma once

#include "console_interface.h"
#include "data_pool.h"
#include "options.h"
#include <functional>
#include <unordered_map>

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

inline void clear(IConsole& console)
{
	console.print("\033[2J\033[1;1H"); // clear the console
}

int runApplication(Options& opts, DataPool& data, IConsole& console);

class MenuItem {
private:
	command _command;
public:
	MenuItem() = default;
	explicit MenuItem(command command);
	bool execute(std::vector<std::string>& command_args, Options& opts, DataPool& data, IConsole& console);
};

class Menu {
private:
	std::unordered_map<std::string, MenuItem> _items;
public:
	Menu();
	bool execute(const std::string& command_text, std::vector<std::string>& command_args, Options& opts, DataPool& data, IConsole& console);
	bool exists(const std::string& command_text);
};