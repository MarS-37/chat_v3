#pragma once
#include "SHA256.h"
#include "mysql.h"

#include <iostream>
#include <string>

class ChatUser final {
public:
	ChatUser(unsigned user_id, const std::string& login, const std::string& password, const std::string& name);

	// getters
	const std::string& getLogin() const;
	const std::string& getPassword() const;
	const std::string& getName() const;
	const std::string& getIp() const;
	void setIp(const std::string &ip);
	void setPort(unsigned short port);
	void setPid(pid_t pid);
	unsigned short getPort() const;
	pid_t getPid() const;
	unsigned getUserId() const;
	bool isLoggedIn() const;
	void login(Mysql &mysql, const std::string &ip, unsigned short port, unsigned short pid);
	void logout(Mysql &mysql);
	void save(Mysql &mysql) const;
	void setLoggedIn();
	void setLoggedOut();

private:
	std::string login_;
	std::string password_;
	std::string name_;
	unsigned user_id_;
	pid_t pid_{ 0 };
	std::string ip_;
	unsigned short port_{ 0 };
	bool isLoggedIn_{ false };
};

