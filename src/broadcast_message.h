#pragma once
#include "chat_message.h"
#include "chat_user.h"

#include <map>
#include <string>
#include <vector>

class BroadcastMessage final : public ChatMessage {
public:
	BroadcastMessage(const std::string &, const std::string &, const std::map<std::string, ChatUser> &);
	BroadcastMessage(const std::string &, const std::string &, const std::map<std::string, ChatUser> &, const std::string &);

	// print the message
	void print() const override;

	// print the message if it is unread by user
	void printIfUnreadByUser(const std::string &) override;

	// check if message is read
	bool isRead() const override;
  
	// save message to database
	void save(Mysql &mysql) const override;
	
	// save message to file
	void save(const std::string&) const override;

	// pack string with message information for transferring it through a network
	std::string createTransferString() const override;

private:

	std::map<std::string, ChatUser> users_unread_;
};
