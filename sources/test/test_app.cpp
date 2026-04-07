#include <gtest/gtest.h>
#include "../options.h"
#include "../application.h"
#include "mock_console.h"

TEST(ApplicationTest, HelpCommand) {
	std::shared_ptr<Options> opts = std::make_shared<Options>();
    std::shared_ptr<MockConsole> console = std::make_shared<MockConsole>();    
    
    console->addInputLine("help");
    console->addInputLine("quit");
    
    runApplication(opts, console);
    
    auto output = console->getOutput();
    bool found_help = false;
    for (const auto& line : output) {
        if (line.contains("COMMANDS")) {
            found_help = true;
            break;
        }
    }
    EXPECT_TRUE(found_help);
}

TEST(ApplicationTest, UnknownCommand) {

	std::shared_ptr<Options> opts = std::make_shared<Options>();
    std::shared_ptr<MockConsole> console = std::make_shared<MockConsole>();    
    
    console->addInputLine("unknown_command");
    console->addInputLine("quit");
    
    runApplication(opts, console);
    
    auto output = console->getOutput();
    bool found_error = false;
    for (const auto& line : output) {
        if (line.contains("Unknown command!")) {
            found_error = true;
            break;
        }
    }
    EXPECT_TRUE(found_error);
}

TEST(ApplicationTest, EmptyInput) {
	std::shared_ptr<Options> opts = std::make_shared<Options>();
    std::shared_ptr<MockConsole> console = std::make_shared<MockConsole>();
    
    console->addInputLine("");
    console->addInputLine("quit");
    
    runApplication(opts, console);
    
    EXPECT_FALSE(console->hasMoreInput());
}