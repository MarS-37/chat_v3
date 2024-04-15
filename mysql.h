#pragma once

#include <string>
#include <vector>
#include <list>

extern "C" {
	#include <mysql.h>
}

class Mysql {
public:
	Mysql();
	void open(
		const std::string &dbname,
		const std::string &dbhost,
		const std::string &dbuser,
		const std::string &dbpassword);
	bool query(const std::string &req);
	const std::string &getError() const;
	std::list<std::vector<std::string>> &fetchAll();

	~Mysql();

private:
	bool connection_active_{ false };
	MYSQL connfd_;
	MYSQL_RES *result_;
	std::string error_;
	std::list<std::vector<std::string>> fields_;
};
