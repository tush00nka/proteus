#pragma once

#include <iostream>
#include <string>


class IConsole {
public:
	virtual ~IConsole() = default;
	virtual std::string readLine() = 0; 
	virtual void print(const std::string& text) = 0;
	virtual void printLine(const std::string& text) = 0;
};

class RealConsole : public IConsole {
public:
	std::string readLine() override {
		std::string line;
		std::getline(std::cin, line);
		return line;
	}

	void print(const std::string& text) override {
		std::cout << text;
	}

	void printLine(const std::string& text) override {
        std::cout << text << "\n";
    }
};