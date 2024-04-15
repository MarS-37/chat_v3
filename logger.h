#pragma once
#include <iostream>
#include <string>
#include <shared_mutex>
#include <fstream>

class Logger {
public:
	Logger(const std::string &filename);
	~Logger();
	void write(const std::string &line);
	bool isEof() const;
	friend void operator<<(Logger &logger, const std::string &line);
	std::string readline();

private:
	std::fstream file_;
	std::shared_mutex mutex_;
};

