#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <mutex>
#include <thread>

enum class LogLevel : std::uint8_t
{
	TRACE,
	WARNING,
	ERROR,
	INFO,
	FATAL,
	USER_INPUT,
	DEBUG
};

inline std::string logTypeToString(LogLevel type)
{
    switch(type)
	{
        case LogLevel::TRACE:     return "TRACE";
        case LogLevel::DEBUG:     return "DEBUG";
        case LogLevel::INFO:      return "INFO";
        case LogLevel::WARNING:   return "WARNING";
        case LogLevel::ERROR:     return "ERROR";
        case LogLevel::FATAL:     return "FATAL";
        case LogLevel::USER_INPUT: return "USER_INPUT";
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

		auto time_point = std::chrono::system_clock::now();
		auto local_time_point = std::chrono::current_zone()->to_local(time_point);

        ss << std::format("{:%Y-%m-%d %H:%M:%S}", local_time_point);
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

	void log(LogLevel type, std::string_view message, const std::string& file = "", int line = 0)
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

	void trace(std::string_view message, const std::string& file = "", int line = 0)
	{
        log(LogLevel::TRACE, message, file, line);
    }
    
    void debug(std::string_view message, const std::string& file = "", int line = 0)
	{
        log(LogLevel::DEBUG, message, file, line);
    }
    
    void info(std::string_view message, const std::string& file = "", int line = 0)
	{
        log(LogLevel::INFO, message, file, line);
    }
    
    void warning(std::string_view message, const std::string& file = "", int line = 0)
	{
        log(LogLevel::WARNING, message, file, line);
    }
    
    void error(std::string_view message, const std::string& file = "", int line = 0)
	{
        log(LogLevel::ERROR, message, file, line);
    }
    
    void fatal(std::string_view message, const std::string& file = "", int line = 0)
	{
        log(LogLevel::FATAL, message, file, line);
    }
    
    void userInput(std::string_view message, const std::string& file = "", int line = 0)
	{
        log(LogLevel::USER_INPUT, message, file, line);
    }
};

template<LogLevel Level>
constexpr void log(Logger& logger, std::string_view msg, const char* file = __builtin_FILE(), int line = __builtin_LINE())
{
	switch(Level)
	{
		case LogLevel::TRACE:
			logger.trace(msg, file, line);
			break;
		case LogLevel::DEBUG:
			logger.debug(msg, file, line);
			break;	
		case LogLevel::INFO:
			logger.info(msg, file, line);
			break;
		case LogLevel::WARNING:
			logger.warning(msg, file, line);
			break;
		case LogLevel::ERROR:
			logger.error(msg, file, line);
			break;
		case LogLevel::FATAL:
			logger.fatal(msg, file, line);
			break;
		case LogLevel::USER_INPUT:
			logger.userInput(msg, file, line);
			break;
		default:
			logger.trace(msg, file, line);
	}
}
