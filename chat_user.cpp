#include "chat_user.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

// construct
ChatUser::ChatUser(const unsigned user_id, const std::string& login, const std::string& password, const std::string& name)
	: user_id_{ user_id }, login_{ login }, password_{ password }, name_{ name } {}

// Getters
const std::string& ChatUser::getLogin() const { return login_; }
const std::string& ChatUser::getPassword() const { return password_; }
const std::string& ChatUser::getName() const { return name_; }
const std::string& ChatUser::getIp() const { return ip_; }
unsigned short ChatUser::getPort() const { return port_; }
pid_t ChatUser::getPid() const { return pid_; }


void ChatUser::setIp(const std::string &ip) {
	ip_ = ip;
}

void ChatUser::setPort(const unsigned short port) {
	port_ = port;
}

void ChatUser::setPid(const pid_t pid) {
	pid_ = pid;
}

void ChatUser::setLoggedIn() {
	isLoggedIn_ = true;
}

void ChatUser::setLoggedOut() {
	isLoggedIn_ = false;
}

void ChatUser::save(Mysql &mysql) const{
			std::stringstream ss;
			ss << "INSERT INTO `users` "
				"(`id`, `login`, `password_hash`, `name`)"
				"VALUES"
				"(" << user_id_ << ", '" << login_ << "', '" << password_ << "', '" << name_ << "');";
			mysql.query(ss.str());
}

void ChatUser::login(Mysql &mysql, const std::string &ip, const unsigned short port, const unsigned short pid) {
	std::stringstream ss;
	ip_ = ip;
	port_ = port;
	pid_ = pid;
	ss << "UPDATE `users` SET "
			"`last_login` = CURRENT_TIMESTAMP "
		"WHERE "
			"`id` = " << user_id_;
	mysql.query(ss.str());
	ss.str(std::string{});
	ss << "DELETE FROM `active_sessions` WHERE `user_id` = " << user_id_;
	mysql.query(ss.str());
	ss.str(std::string{});
	ss << "INSERT INTO `active_sessions` ( "
			"`user_id`, "
			"`ip`, "
			"`pid`, "
			"`port` "
		") VALUES ("
			<< user_id_ << ", "
			"INET_ATON('" << ip << "'), "
			<< pid << ", "
			<< port
		<< ")";
	mysql.query(ss.str());
	setLoggedIn();
}

void ChatUser::logout(Mysql &mysql) {
	std::stringstream ss;
	ss << "DELETE FROM `active_sessions` WHERE `user_id` = " << user_id_;
	mysql.query(ss.str());
	setLoggedOut();
}

bool ChatUser::isLoggedIn() const {
	return isLoggedIn_;
}

unsigned ChatUser::getUserId() const {
	return user_id_;
}
