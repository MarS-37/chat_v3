#pragma once
#include "chat_message.h"

#include <string>

class PrivateMessage final : public ChatMessage {
public:
	PrivateMessage(const std::string &sender, const std::string &receiver, const std::string &text, bool read = false);

	// print the message
	void print() const override;

	// print the message if it is unread by user
	void printIfUnreadByUser(const std::string &) override;

	// check if message is not read
	bool isRead() const override;

	// save message to database
	void save(Mysql &mysql) const override;
	
	// save message to file
	void save(const std::string&) const override;
	
	// pack string with message information for transferring it through a network
	std::string createTransferString() const override;

private:
	bool read_{ false };
	std::string receiver_;
};
