#pragma once

#include "console_interface.h"
#include "data_pool.h"
#include "options.h"
#include "logger.h"
#include <functional>
#include <unordered_map>

const size_t kInputVectorArgc = 5;
const size_t kAddVectorArgc = 6;

using command = std::function<void(DataPool& data, std::vector<std::string>& commandArgs, Options& opts, IConsole& console, Logger& logger)>;

void inputType(DataPool& data, std::vector<std::string>& commandArgs, Options& opts, IConsole& console, Logger& logger);
void inputVec(DataPool& data, std::vector<std::string>& commandArgs, Options& opts, IConsole& console, Logger& logger);
void addVec(DataPool& data, std::vector<std::string>& commandArgs, Options& opts, IConsole& console, Logger& logger);
void setUsername(DataPool& data, std::vector<std::string>& commandArgs, Options& opts, IConsole& console, Logger& logger);
void quitProgram(DataPool& data, std::vector<std::string>& commandArgs, Options& opts, IConsole& console, Logger& logger);
void help(DataPool& data, std::vector<std::string>& commandArgs, Options& opts, IConsole& console, Logger& logger);
void popVec(DataPool& data, std::vector<std::string>& commandArgs, Options& opts, IConsole& console, Logger& logger);

static void clear(IConsole& console)
{
    console.print("\033[2J\033[1;1H"); // clear the console
}

int runApplication(Options& opts, DataPool& data, IConsole& console, Logger& logger);

class MenuItem {
private:
    command _command;
public:
	MenuItem() = default;
    explicit MenuItem(command command);
    bool execute(std::vector<std::string>& command_args, Options& opts, DataPool& data, IConsole& console, Logger& logger);
};

class Menu {
private:
    std::unordered_map<std::string, MenuItem> _items;
public:
    Menu();
    bool execute(const std::string& command_text, std::vector<std::string>& command_args, Options& opts, DataPool& data, IConsole& console, Logger& logger);
    bool exists(const std::string& command_text);
};