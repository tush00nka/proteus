#include <gtest/gtest.h>
#include "../options.h"
#include "../vector4.h"
#include "mock_console.h"
#include "../application.h"

TEST(ApplicationTest, HelpCommand) {
    Options opts(0, nullptr);
    Vector4 vec;
    MockConsole console;
    
    console.addInputLine("help");
    console.addInputLine("quit");
    
    runApplication(opts, vec, console);
    
    auto output = console.getOutput();
    bool found_help = false;
    for (const auto& line : output) {
        if (line.find("AVAILABLE COMMANDS:") != std::string::npos) {
            found_help = true;
            break;
        }
    }
    EXPECT_TRUE(found_help);
}

TEST(ApplicationTest, SetVectorType) {
    Options opts(0, nullptr);
    Vector4 vec;
    MockConsole console;
    
    console.addInputLine("type float");
    console.addInputLine("quit");
    
    runApplication(opts, vec, console);
    
    EXPECT_EQ(vec.getType(), "float");
}

TEST(ApplicationTest, ViewCurrentType) {
    Options opts(0, nullptr);
    Vector4 vec;
    vec.setType("float");
    MockConsole console;
    
    console.addInputLine("type");
    console.addInputLine("quit");
    
    runApplication(opts, vec, console);
    
    auto output = console.getOutput();
    bool found_type = false;
    for (const auto& line : output) {
        if (line.find("Currently set type: float") != std::string::npos) {
            found_type = true;
            break;
        }
    }
    EXPECT_TRUE(found_type);
}

TEST(ApplicationTest, SetVectorValues) {
    Options opts(0, nullptr);
    Vector4 vec;
    vec.setType("float");
    MockConsole console;
    
    console.addInputLine("vec 1.5 2.7 3.14 4.2");
    console.addInputLine("quit");
    
    runApplication(opts, vec, console);
    
    auto output = console.getOutput();
    bool found_update = false;
    for (const auto& line : output) {
        if (line.find("Vector data updated!") != std::string::npos) {
            found_update = true;
            break;
        }
    }
    EXPECT_TRUE(found_update);
    
    std::string vec_str = vec.sprint();
    EXPECT_NE(vec_str.find("1.5"), std::string::npos);
}

TEST(ApplicationTest, ChangeRole) {
    Options opts(0, nullptr);
    Vector4 vec;
    MockConsole console;
    
    console.addInputLine("role admin");
    console.addInputLine("quit");
    
    runApplication(opts, vec, console);
    
    EXPECT_EQ(opts.getRole(), "admin");
}

TEST(ApplicationTest, UnknownCommand) {
    Options opts(0, nullptr);
    Vector4 vec;
    MockConsole console;
    
    console.addInputLine("unknown_command");
    console.addInputLine("quit");
    
    runApplication(opts, vec, console);
    
    auto output = console.getOutput();
    bool found_error = false;
    for (const auto& line : output) {
        if (line.find("Unknown command!") != std::string::npos) {
            found_error = true;
            break;
        }
    }
    EXPECT_TRUE(found_error);
}

TEST(ApplicationTest, EmptyInput) {
    Options opts(0, nullptr);
    Vector4 vec;
    MockConsole console;
    
    console.addInputLine("");
    console.addInputLine("quit");
    
    runApplication(opts, vec, console);
    
    EXPECT_FALSE(console.hasMoreInput());
}

TEST(ApplicationTest, IncompleteVecCommand) {
    Options opts(0, nullptr);
    Vector4 vec;
    MockConsole console;
    
    console.addInputLine("vec 1 2 3"); // Less args than needed
    console.addInputLine("quit");
    
    runApplication(opts, vec, console);
    
    auto output = console.getOutput();
    bool found_error = false;
    for (const auto& line : output) {
        if (line.find("Not enough arguments!") != std::string::npos) {
            found_error = true;
            break;
        }
    }
    EXPECT_TRUE(found_error);
}