#include <gtest/gtest.h>
#include "../options.h"
#include "../data_pool.h"
#include "../application.h"
#include "mock_console.h"

TEST(ApplicationTest, HelpCommand) {
	std::shared_ptr<Options> opts = std::make_shared<Options>(0, nullptr);
    DataPool data;
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

TEST(ApplicationTest, SetVectorType) {

	std::shared_ptr<Options> opts = std::make_shared<Options>(0, nullptr);
	std::shared_ptr<MockConsole> console = std::make_shared<MockConsole>(); 
    
	console->addInputLine("add int 1 2 3 4");
    console->addInputLine("type float");
    console->addInputLine("quit");
    
    runApplication(opts, console);
    
    auto output = console->getOutput();
    bool found_type = false;
    for (const auto& line : output) {
        if (line.contains("Set type: float")) {
            found_type = true;
            break;
        }
    }
    EXPECT_TRUE(found_type);
}

TEST(ApplicationTest, ViewCurrentType) {
	
	std::shared_ptr<Options> opts = std::make_shared<Options>(0, nullptr);
	std::shared_ptr<MockConsole> console = std::make_shared<MockConsole>(); 
    
	console->addInputLine("add float 1 2 3 4");
    console->addInputLine("type");
    console->addInputLine("quit");
    
    runApplication(opts, console);
    
    auto output = console->getOutput();
    bool found_type = false;
    for (const auto& line : output) {
        if (line.contains("Currently set type: float")) {
            found_type = true;
            break;
        }
    }
    EXPECT_TRUE(found_type);
}

TEST(ApplicationTest, SetVectorValues) {

	std::shared_ptr<Options> opts = std::make_shared<Options>(0, nullptr);
	std::shared_ptr<MockConsole> console = std::make_shared<MockConsole>(); 

	console->addInputLine("add float 1 2 3 4");
    console->addInputLine("vec 1.5 2.7 3.14 4.2");
    console->addInputLine("quit");
    
    runApplication(opts, console);
    
    auto output = console->getOutput();
    bool found_update = false;
    for (const auto& line : output) {
        if (line.contains("Vector data updated!")) {
            found_update = true;
            break;
        }
    }
    EXPECT_TRUE(found_update);
}

TEST(ApplicationTest, ChangeUsername) {

	std::shared_ptr<Options> opts = std::make_shared<Options>(0, nullptr);
    std::shared_ptr<MockConsole> console = std::make_shared<MockConsole>();  

    console->addInputLine("username admin");
    console->addInputLine("quit");
    
    runApplication(opts, console);
    
    EXPECT_EQ(opts->getUsername(), "admin");
}

TEST(ApplicationTest, UnknownCommand) {

	std::shared_ptr<Options> opts = std::make_shared<Options>(0, nullptr);
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

    

	std::shared_ptr<Options> opts = std::make_shared<Options>(0, nullptr);
    std::shared_ptr<MockConsole> console = std::make_shared<MockConsole>();
    
    console->addInputLine("");
    console->addInputLine("quit");
    
    runApplication(opts, console);
    
    EXPECT_FALSE(console->hasMoreInput());
}

TEST(ApplicationTest, IncompleteVecCommand) {

	std::shared_ptr<Options> opts = std::make_shared<Options>(0, nullptr);
    std::shared_ptr<MockConsole> console = std::make_shared<MockConsole>();
    
    console->addInputLine("add int 1 2 3"); // Less args than needed
    console->addInputLine("quit");
    
    runApplication(opts, console);
    
    auto output = console->getOutput();
    bool found_error = false;
    for (const auto& line : output) {
        if (line.contains("Not enough arguments!")) {
            found_error = true;
            break;
        }
    }
    EXPECT_TRUE(found_error);
}