#include "mysql.h"

#include <sstream>
#include <stdexcept>
#include <iostream>
#include <iomanip>

Mysql::Mysql() {
	mysql_init(&connfd_);
	if (&connfd_ == nullptr) {
		throw std::runtime_error{ "can't create MySQL descriptor" };
	}
}

void Mysql::open(
	const std::string &dbname,
	const std::string &dbhost,
	const std::string &dbuser,
	const std::string &dbpassword
	) {
	std::string charset{ "utf8mb4" };
	connection_active_ = mysql_real_connect(&connfd_, dbhost.c_str(), dbuser.c_str(), dbpassword.c_str(), dbname.c_str(), 0, nullptr, 0);
	if (!connection_active_) {
		std::stringstream ss;
		ss << "can't connect to database (" << mysql_error(&connfd_);
		mysql_close(&connfd_);
		throw std::runtime_error{ ss.str() };
	}
	mysql_set_character_set(&connfd_, charset.c_str());
	auto actual_charset = mysql_character_set_name(&connfd_);
	if (charset != actual_charset) {
		mysql_close(&connfd_);
		throw std::runtime_error{ "can't set charset" };
	}
	
}

bool Mysql::query(const std::string &req) {
	error_.clear();
	mysql_query(&connfd_, req.c_str());
	result_ = mysql_store_result(&connfd_);
	if (mysql_error(&connfd_)) {
		error_ = mysql_error(&connfd_);
	}
	return error_.empty();
}

const std::string &Mysql::getError() const {
	return error_;
}
std::list<std::vector<std::string>> &Mysql::fetchAll() {
	fields_.clear();
	if (result_ == nullptr) {
		return fields_;
	}
	while (auto row = mysql_fetch_row(result_)) {
		std::vector<std::string> row_array;
		for (unsigned i = 0; i < mysql_num_fields(result_); ++i) {
			if (row[i] == nullptr) {
				row_array.emplace_back(std::string{});
			}
			else {
				row_array.emplace_back(row[i]);
			}
		}
		fields_.push_back(std::move(row_array));
	}

	return fields_;
}

Mysql::~Mysql() {
	mysql_close(&connfd_);
}
