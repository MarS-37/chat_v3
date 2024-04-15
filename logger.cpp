#include "logger.h"
#include <iomanip>
#include <ctime>
#include <stdexcept>

Logger::Logger(const std::string &filename) {
	file_.open(filename, std::ios::in | std::ios::out | std::ios::app);
	if (!file_.is_open()) {
		throw std::runtime_error{ "Error: can not open log file!" };
	}
}

void Logger::write(const std::string &line) {
	auto t{ std::time(nullptr) };
	auto tm{ *std::localtime(&t) };

	mutex_.lock();
	file_ << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << ": " << line << std::endl;
	mutex_.unlock();
}

bool Logger::isEof() const {
	return file_.eof();
}

std::string Logger::readline() {
	if (file_.eof()) {
		return std::string{};
	}

	std::string result;
	mutex_.lock_shared();
	getline(file_, result);
	mutex_.unlock_shared();

	return result;
}

Logger::~Logger() {
	file_.close();
}

void operator<<(Logger &logger, const std::string &line) {
	logger.write(line);
}
