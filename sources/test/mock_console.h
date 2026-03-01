#pragma once
#include <string>
#include <vector>
#include "../console_interface.h"

class MockConsole : public IConsole {
private:
    std::vector<std::string> _input_lines;
    std::vector<std::string> _output_lines;
    size_t _current_input_index = 0;
    
public:
    void addInputLine(const std::string& line) {
        _input_lines.push_back(line);
    }
    
    std::string readLine() override {
        if (_current_input_index < _input_lines.size()) {
            return _input_lines[_current_input_index++];
        }
        return "";
    }
    
    void print(const std::string& text) override {
        if (!_output_lines.empty() && !_output_lines.back().empty() && 
            _output_lines.back().back() != '\n') {
            _output_lines.back() += text;
        } else {
            _output_lines.push_back(text);
        }
    }
    
    void printLine(const std::string& text) override {
        _output_lines.push_back(text);
    }
    
    [[nodiscard]] std::vector<std::string> getOutput() const {
        return _output_lines;
    }
    
    void clearOutput() {
        _output_lines.clear();
    }
    
    bool hasMoreInput() const {
        return _current_input_index < _input_lines.size();
    }
};