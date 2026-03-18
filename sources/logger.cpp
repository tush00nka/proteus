#include "logger.h"

void Logger::log(LogLevel type, std::string_view message, const std::string& file, int line)
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

void Logger::clear(const std::string& filename) {
	std::lock_guard<std::mutex> lock(this->_log_mutex);
	if (this->_log_file.is_open()) {
		this->_log_file.close();
	}
	this->_log_file.open(filename, std::ios::trunc);
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