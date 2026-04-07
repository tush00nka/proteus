#pragma once

#include "console_interface.h"
#include "options.h"
#include <memory>
#include <unordered_map>
#include <utility>

const size_t kInputVectorArgc = 5;
const size_t kAddVectorArgc = 6;

void runApplication(const std::shared_ptr<Options>& opts, const std::shared_ptr<IConsole>& console);

class MenuItem {
private:
public:
	MenuItem() = default;
	virtual ~MenuItem() = default;
    virtual bool execute(const std::vector<std::string_view>& args) = 0;
};

class Help : public MenuItem {
private:
	std::shared_ptr<IConsole> _console;
public:
	explicit Help(std::shared_ptr<IConsole> console) : _console(std::move(console)) {}
	bool execute(const std::vector<std::string_view>& args) final;
};

class Quit : public MenuItem {
private:
	std::shared_ptr<bool> _should_exit;
public:
	explicit Quit(std::shared_ptr<bool> should_exit) : _should_exit(std::move(should_exit)) {};
	bool execute(const std::vector<std::string_view>& args) final;
};

// class Send : public MenuItem {
// private:
// 	std::shared_ptr<DataPool> _data_pool;
// 	std::shared_ptr<Options> _options;
// 	std::shared_ptr<IConsole> _console;
// public:
// 	explicit Send(
// 		std::shared_ptr<DataPool> data_pool,
// 		std::shared_ptr<Options> options,
// 		std::shared_ptr<IConsole> console
// 	) : 
// 		_data_pool(std::move(data_pool)),
// 		_options(std::move(options)),
// 		_console(std::move(console)) {};

// 	bool execute(const std::vector<std::string_view>& args) final;
// };

class Move : public MenuItem {
private:
	std::shared_ptr<Options> _options;
	std::shared_ptr<IConsole> _console;
public:
	explicit Move(
		std::shared_ptr<Options> options,
		std::shared_ptr<IConsole> console
	) : 
		_options(std::move(options)),
		_console(std::move(console)) {};

	bool execute(const std::vector<std::string_view>& args) final;
};

class Menu {
private:
	std::shared_ptr<bool> _should_exit = std::make_shared<bool>(false);

	std::shared_ptr<IConsole> _console;
	std::shared_ptr<Options> _options;

    std::unordered_map<std::string, std::unique_ptr<MenuItem>> _items;
public:
	explicit Menu(
		std::shared_ptr<IConsole> console,
		std::shared_ptr<Options> options
	) : 
		_console(std::move(console)),
		_options(std::move(options))
	{
		_items.emplace("help", std::make_unique<Help>(_console));

		_items.emplace("exit", std::make_unique<Quit>(_should_exit));
		_items.emplace("quit", std::make_unique<Quit>(_should_exit));
		_items.emplace("q", std::make_unique<Quit>(_should_exit));

		// _items.emplace("send", std::make_unique<Send>(_data_pool, _options, _console));

		// new ones
		_items.emplace("move", std::make_unique<Move>(_options, _console));
	}
	
    bool execute(std::string_view command_text, const std::vector<std::string_view>& args);
    bool exists(std::string_view command_text);
	bool getShouldExit() const;
};