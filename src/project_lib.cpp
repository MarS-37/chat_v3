#include "project_lib.h"
#include <iostream>

// split string to vector
std::vector<std::string> Chat::split(const std::string &src, const std::string &delimiter) {
	size_t pos = 0;
	std::string src_copy { src };
	std::vector<std::string> result;

	while ((pos = src_copy.find(delimiter)) != std::string::npos) {
		result.emplace_back(src_copy.substr(0, pos));
		src_copy.erase(0, pos + delimiter.length());
	}
	if (!src_copy.empty()) {
		result.push_back(src_copy);
	}
	return result;
}

