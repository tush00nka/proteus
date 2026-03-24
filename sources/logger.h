#pragma once

#include <string>
#include <fstream>
#include <mutex>

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

constexpr std::string logTypeToString(LogLevel type)
{
    switch(type)
	{
        case LogLevel::TRACE:      return "TRACE";
        case LogLevel::DEBUG:      return "DEBUG";
        case LogLevel::INFO:       return "INFO";
        case LogLevel::WARNING:    return "WARNING";
        case LogLevel::ERROR:      return "ERROR";
        case LogLevel::FATAL:      return "FATAL";
        case LogLevel::USER_INPUT: return "USER_INPUT";
        default: return "UNKNOWN";
    }
}

const int kMilliseconds = 1000;

class Logger {
private:
	static std::ofstream log_file;
	static std::mutex log_mutex;
	static bool console_output;

public:
	Logger() = delete;
    ~Logger() = delete;

	static void init(const std::string& filename = "proteus.log", bool console = false);
	static void shutdown();

	static void log      (LogLevel type, std::string_view message, const std::string& file, int line);
	static void clear    (const std::string& filename = "proteus.log");

	static void trace    (std::string_view message, const std::string& file, int line);
	static void debug    (std::string_view message, const std::string& file, int line);
	static void info     (std::string_view message, const std::string& file, int line);
	static void warning  (std::string_view message, const std::string& file, int line);
	static void error    (std::string_view message, const std::string& file, int line);
	static void fatal    (std::string_view message, const std::string& file, int line);
	static void userInput(std::string_view message, const std::string& file, int line);
};

template<LogLevel Level>
constexpr void log(std::string_view msg, const char* file = __builtin_FILE(), int line = __builtin_LINE())
{
	switch(Level)
	{
		case LogLevel::TRACE:
			Logger::trace(msg, file, line);
			break;
		case LogLevel::DEBUG:
			Logger::debug(msg, file, line);
			break;	
		case LogLevel::INFO:
			Logger::info(msg, file, line);
			break;
		case LogLevel::WARNING:
			Logger::warning(msg, file, line);
			break;
		case LogLevel::ERROR:
			Logger::error(msg, file, line);
			break;
		case LogLevel::FATAL:
			Logger::fatal(msg, file, line);
			break;
		case LogLevel::USER_INPUT:
			Logger::userInput(msg, file, line);
			break;
		default:
			Logger::trace(msg, file, line);
	}
}
