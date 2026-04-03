#pragma once

#include "console_interface.h"
#include "data_pool.h"
#include "options.h"
#include <memory>
#include <unordered_map>
#include <utility>

const size_t kInputVectorArgc = 5;
const size_t kAddVectorArgc = 6;

int runApplication(const std::shared_ptr<Options>& opts, const std::shared_ptr<IConsole>& console);

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
	std::shared_ptr<Options> _options;
public:
	explicit Quit(std::shared_ptr<Options> options) : _options(std::move(options)) {};
	bool execute(const std::vector<std::string_view>& args) final;
};

class SetUsername : public MenuItem {
private:
	std::shared_ptr<Options> _options;
	std::shared_ptr<IConsole> _console;
public:
	explicit SetUsername(
		std::shared_ptr<Options> options,
		std::shared_ptr<IConsole> console
	) : 
		_options(std::move(options)),
		_console(std::move(console)) {};

	bool execute(const std::vector<std::string_view>& args) final;
};

class Send : public MenuItem {
private:
	std::shared_ptr<DataPool> _data_pool;
	std::shared_ptr<Options> _options;
	std::shared_ptr<IConsole> _console;
public:
	explicit Send(
		std::shared_ptr<DataPool> data_pool,
		std::shared_ptr<Options> options,
		std::shared_ptr<IConsole> console
	) : 
		_data_pool(std::move(data_pool)),
		_options(std::move(options)),
		_console(std::move(console)) {};

	bool execute(const std::vector<std::string_view>& args) final;
};

class InputType : public MenuItem {
private:
	std::shared_ptr<DataPool> _data_pool;
	std::shared_ptr<IConsole> _console;
public:
	explicit InputType(
		std::shared_ptr<DataPool> data_pool,
		std::shared_ptr<IConsole> console
	) :
		_data_pool(std::move(data_pool)),
		_console(std::move(console)) {};
	bool execute(const std::vector<std::string_view>& args) final;
};

class PopVec : public MenuItem {
private:
	std::shared_ptr<DataPool> _data_pool;
	std::shared_ptr<IConsole> _console;
public:
	explicit PopVec(
		std::shared_ptr<DataPool> data_pool,
		std::shared_ptr<IConsole> console
	) :
		_data_pool(std::move(data_pool)),
		_console(std::move(console)) {};
	bool execute(const std::vector<std::string_view>& args) final;
};

class InputVec : public MenuItem {
private:
	std::shared_ptr<DataPool> _data_pool;
	std::shared_ptr<IConsole> _console;
public:
	explicit InputVec(
		std::shared_ptr<DataPool> data_pool,
		std::shared_ptr<IConsole> console
	) :
		_data_pool(std::move(data_pool)),
		_console(std::move(console)) {};
	bool execute(const std::vector<std::string_view>& args) final;
};

class AddVec : public MenuItem {
private:
	std::shared_ptr<DataPool> _data_pool;
	std::shared_ptr<IConsole> _console;
public:
	explicit AddVec(
		std::shared_ptr<DataPool> data_pool,
		std::shared_ptr<IConsole> console
	) :
		_data_pool(std::move(data_pool)),
		_console(std::move(console)) {};
	bool execute(const std::vector<std::string_view>& args) final;
};

class TestAccessibility : public MenuItem {
private:
	std::shared_ptr<IConsole> _console;
public:
	explicit TestAccessibility(std::shared_ptr<IConsole> console ) : _console(std::move(console)) {};
	bool execute(const std::vector<std::string_view>& args) final;
};


class Menu {
private:
	std::shared_ptr<DataPool> _data_pool;
	std::shared_ptr<IConsole> _console;
	std::shared_ptr<Options> _options;

    std::unordered_map<std::string, std::unique_ptr<MenuItem>> _items;
public:
	explicit Menu(
		std::shared_ptr<DataPool> data_pool,
		std::shared_ptr<IConsole> console,
		std::shared_ptr<Options> options
	) : 
		_data_pool(std::move(data_pool)),
		_console(std::move(console)),
		_options(std::move(options))
	{
		_items.emplace("help", std::make_unique<Help>(_console));

		_items.emplace("exit", std::make_unique<Quit>(_options));
		_items.emplace("quit", std::make_unique<Quit>(_options));
		_items.emplace("q", std::make_unique<Quit>(_options));

		_items.emplace("username", std::make_unique<SetUsername>(_options, _console));

		_items.emplace("type", std::make_unique<InputType>(_data_pool, _console));
		_items.emplace("pop", std::make_unique<PopVec>(_data_pool, _console));
		_items.emplace("vec", std::make_unique<InputVec>(_data_pool, _console));
		_items.emplace("add", std::make_unique<AddVec>(_data_pool, _console));

		_items.emplace("test", std::make_unique<TestAccessibility>( _console));

		_items.emplace("send", std::make_unique<Send>(_data_pool, _options, _console));
	}
	
    bool execute(std::string_view command_text, const std::vector<std::string_view>& args);
    bool exists(std::string_view command_text);
};