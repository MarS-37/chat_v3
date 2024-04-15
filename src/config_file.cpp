#include "config_file.h"

#include <algorithm>

ConfigFile::ConfigFile(const std::string &filename) {
	std::ifstream fs(filename, std::ios::in);
	if (!fs.is_open()) {
		throw std::runtime_error{ "Cannot open file '" + filename + "' for read" };
	}
	std::string buf;
	while (!fs.eof()) {
		getline(fs, buf);
		// Removing space characters from string
		std::erase(buf, ' ');
		std::erase(buf, '\t');
		// Removing comments
		auto octotorp = buf.find('#');
		if (octotorp != std::string::npos) {
			buf.erase(octotorp, buf.length() - octotorp);
		}
		auto tokens = Chat::split(buf, "=");
		if (tokens.size() >= 2 && options_.find(tokens[0]) == options_.end()) {
			options_.insert(make_pair(tokens[0], tokens[1]));
		}
	}
	fs.close();
}

const std::string &ConfigFile::operator[](const std::string &index) const {
	return options_.at(index);
}

