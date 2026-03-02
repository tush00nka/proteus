#pragma once

#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <mutex>
#include <thread>

enum class LogType
{
	TRACE,
	WARNING,
	ERROR,
	INFO,
	FATAL,
	USER_INPUT,
	DEBUG
};

inline std::string logTypeToString(LogType type)
{
    switch(type)
	{
        case LogType::TRACE:     return "TRACE";
        case LogType::DEBUG:     return "DEBUG";
        case LogType::INFO:      return "INFO";
        case LogType::WARNING:   return "WARNING";
        case LogType::ERROR:     return "ERROR";
        case LogType::FATAL:     return "FATAL";
        case LogType::USER_INPUT: return "USER_INPUT";
        default: return "UNKNOWN";
    }
}

const int kMilliseconds = 1000;

class Logger {
private:
	std::ofstream _log_file;
	std::mutex _log_mutex;
	bool _console_output;

	static std::string getCurrentDateTime()
	{
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % kMilliseconds;
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
        ss << "." << std::setfill('0') << std::setw(3) << ms.count();
        return ss.str();
    }

	static std::string getThreadId()
	{
        std::stringstream ss;
        ss << std::this_thread::get_id();
        return ss.str();
    }

public:
	explicit Logger(const std::string& filename = "proteus.log", bool console = false) : _console_output(console)
	{
        _log_file.open(filename, std::ios::app);
        if (!_log_file.is_open())
		{
            std::cerr << "Failed to open log file: " << filename << "\n";
        }
    }
    
    ~Logger()
	{
        if (_log_file.is_open())
		{
            _log_file.close();
        }
    }

	Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

	void log(LogType type, const std::string& message, const std::string& file = "", int line = 0)
	{
		std::lock_guard<std::mutex> lock(_log_mutex);

		std::stringstream log_entry;

		log_entry << getCurrentDateTime() << " ";
		log_entry << "[" << logTypeToString(type) << "] ";
		log_entry << getThreadId() << " ";
		log_entry << message << " ";

		if (!file.empty() && line > 0)
		{
            log_entry << "[" << file << ":" << line << "]";
        }

        std::string full_message = log_entry.str();
        
        // Запись в файл
        if (this->_log_file.is_open()) {
            this->_log_file << full_message << "\n";
            this->_log_file.flush();
        }
        
        // Запись в консоль
        if (this->_console_output) {
            std::cout << full_message << "\n";
        }
	}

	void clear() {
        std::lock_guard<std::mutex> lock(this->_log_mutex);
        if (this->_log_file.is_open()) {
            this->_log_file.close();
        }
        this->_log_file.open("proteus.log", std::ios::trunc);
    }

	void trace(const std::string& message, const std::string& file = "", int line = 0)
	{
        log(LogType::TRACE, message, file, line);
    }
    
    void debug(const std::string& message, const std::string& file = "", int line = 0)
	{
        log(LogType::DEBUG, message, file, line);
    }
    
    void info(const std::string& message, const std::string& file = "", int line = 0)
	{
        log(LogType::INFO, message, file, line);
    }
    
    void warning(const std::string& message, const std::string& file = "", int line = 0)
	{
        log(LogType::WARNING, message, file, line);
    }
    
    void error(const std::string& message, const std::string& file = "", int line = 0)
	{
        log(LogType::ERROR, message, file, line);
    }
    
    void fatal(const std::string& message, const std::string& file = "", int line = 0)
	{
        log(LogType::FATAL, message, file, line);
    }
    
    void userInput(const std::string& message, const std::string& file = "", int line = 0)
	{
        log(LogType::USER_INPUT, message, file, line);
    }
};

// automatically add file and line
#define LOG_TRACE(logger, msg) (logger).trace(msg, __FILE__, __LINE__)
#define LOG_DEBUG(logger, msg) (logger).debug(msg, __FILE__, __LINE__)
#define LOG_INFO(logger, msg) (logger).info(msg, __FILE__, __LINE__)
#define LOG_WARNING(logger, msg) (logger).warning(msg, __FILE__, __LINE__)
#define LOG_ERROR(logger, msg) (logger).error(msg, __FILE__, __LINE__)
#define LOG_FATAL(logger, msg) (logger).fatal(msg, __FILE__, __LINE__)
#define LOG_USER(logger, msg) (logger).userInput(msg, __FILE__, __LINE__)