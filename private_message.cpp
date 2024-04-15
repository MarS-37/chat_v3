#include "private_message.h"
#include "chat_message.h"
#include "chat_user.h"

#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

PrivateMessage::PrivateMessage(const std::string &sender, const std::string &receiver, const std::string &text, const bool read) :
	receiver_{ receiver },
	read_ { read } {
	sender_ = sender;
	text_ = text;
}

void PrivateMessage::print() const {
	std::cout << sender_ << ": @" << receiver_ << " " << text_ << std::endl;
}

void PrivateMessage::printIfUnreadByUser(const std::string &user) {
	if (!read_ && (receiver_ == user)) {
		print();
		read_ = true;
	}
}

bool PrivateMessage::isRead() const {
	return read_;
}

void PrivateMessage::save(const std::string &filename) const {
	std::ofstream file(filename, std::ios::out | std::ios::app);
	if (!file.is_open()) {
		throw std::runtime_error{ "Error: cannot open file" + filename + " for append" };
	}
	file << "PRIVATE\n"
		<< sender_ << '\n'
		<< receiver_ << '\n'
		<< (read_ ? "READ" : "UNREAD") << '\n'
		<< text_ << std::endl;
	file.close();
}

void PrivateMessage::save(Mysql &mysql) const {
	std::stringstream ss;
	mysql.query("SELECT COALESCE(max(`id`), -1) FROM `messages`");
	auto rows = mysql.fetchAll();
	unsigned new_id = std::stoi(rows.front().at(0)) + 1;
	ss << "INSERT INTO `messages` (`id`, `type`, `sender`, `receiver`, `text`) VALUES (" <<
		new_id << ", 'PRIVATE', "
		"(SELECT `id` FROM `users` WHERE `login` = '" << sender_ << "'), "
		"(SELECT `id` FROM `users` WHERE `login` = '" << receiver_ << "')"
		", '" << text_ << "')";
	mysql.query(ss.str());
	if (read_) {
		return;
	}

	ss.str(std::string{});
	ss << "INSERT INTO `unread_messages` (`message_id`, `user_id`)"
		"VALUES (" << new_id << ", "
		"(SELECT `id` FROM `users` WHERE `login` = '" << receiver_ << "'))";
	mysql.query(ss.str());
}

std::string PrivateMessage::createTransferString() const {
	return std::string{ "PRIVATE\n" } + sender_ + "\n" + text_ + "\n";
}
