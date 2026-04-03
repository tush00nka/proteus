#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>
#include <sstream>

#include "logger.h"

std::ofstream Logger::log_file;
std::mutex Logger::log_mutex;
bool Logger::console_output = false;

namespace {
	std::string getCurrentDateTime()
	{
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % kMilliseconds;
        
        std::stringstream ss;

		auto time_point = std::chrono::system_clock::now();
		auto local_time_point = std::chrono::current_zone()->to_local(time_point);

        ss << std::format("{:%Y-%m-%d %H:%M:%S}", local_time_point);
        ss << "." << std::setfill('0') << std::setw(3) << ms.count();
        return ss.str();
    }

	std::string getThreadId()
	{
        std::stringstream ss;
        ss << std::this_thread::get_id();
        return ss.str();
    }
} // namespace

void Logger::init(const std::string& filename, bool console)
{
    std::lock_guard<std::mutex> lock(log_mutex);
    console_output = console;
    
    if (log_file.is_open())
    {
        log_file.close();
    }
    
    log_file.open(filename, std::ios::app);
    if (!log_file.is_open())
    {
        std::cerr << "Failed to open log file: " << filename << "\n";
    }
}

void Logger::shutdown()
{
    std::lock_guard<std::mutex> lock(log_mutex);
    if (log_file.is_open())
    {
        log_file.close();
    }
}

void Logger::log(LogLevel type, std::string_view message, const std::string& file, int line)
{
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

	std::lock_guard<std::mutex> lock(log_mutex);

	// Запись в файл
	if (log_file.is_open()) {
		log_file << full_message << "\n";
		log_file.flush();
	}
	
	// Запись в консоль
	if (console_output) {
		std::cout << full_message << "\n";
	}
}

void Logger::clear(const std::string& filename) {
	std::lock_guard<std::mutex> lock(log_mutex);
	if (log_file.is_open()) {
		log_file.close();
	}
	log_file.open(filename, std::ios::trunc);
}

void Logger::trace(std::string_view message, const std::string& file, int line)
{
	log(LogLevel::TRACE, message, file, line);
}

void Logger::debug(std::string_view message, const std::string& file, int line)
{
	log(LogLevel::DEBUG, message, file, line);
}

void Logger::info(std::string_view message, const std::string& file, int line)
{
	log(LogLevel::INFO, message, file, line);
}

void Logger::warning(std::string_view message, const std::string& file, int line)
{
	log(LogLevel::WARNING, message, file, line);
}

void Logger::error(std::string_view message, const std::string& file, int line)
{
	log(LogLevel::ERROR, message, file, line);
}

void Logger::fatal(std::string_view message, const std::string& file, int line)
{
	log(LogLevel::FATAL, message, file, line);
}

void Logger::userInput(std::string_view message, const std::string& file, int line)
{
	log(LogLevel::USER_INPUT, message, file, line);
}