#pragma once

#include "mysql.h"
#include "chat_user.h"
#include "chat_message.h"
#include "broadcast_message.h"
#include "private_message.h"
#include "config_file.h"
#include "logger.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <atomic>

#if defined(_WIN64) or defined(_WIN32)
#include <Windows.h>
struct WindowsVersion {
	unsigned major, minor, build;
};
#elif defined(__linux__)
extern "C" {
	#include <unistd.h>
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <sys/wait.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
}
#endif

class ChatServer final {
public:
	ChatServer(); // constructor
	~ChatServer(); // destructor
	void work(); // main work
	void childDeathHandler(int signum);
	void sigIntHandler(int signum);
	void sigTermHandler(int signum);

private:	
	bool isLoginAvailable(const std::string& login) const; // login availability
	void signUp(); // registration
	bool isValidLogin(const std::string& login) const; // login verification
	void signIn(); // authorization
	void signOut(); // user logout
	void removeUser(); // deleting a user
	void removeUser(const std::string &cmd); // deleting a user
	void sendMessage(); // sending a message
	void sendPrivateMessage(ChatUser& sender, const std::string& receiverName, const std::string& messageText); // sending a private message
	void sendBroadcastMessage(ChatUser& sender, const std::string& message); // sending a shared message
	void checkUnreadMessages(); // check unread messages
	void saveUsers() const;
	void saveMessages() const; // save all messages to file
	void loadUsers(); // load user list from file
	void setUsersInactive() const;
	void loadMessages(const std::string &filename); // load message list from file
	void printSystemInformation() const; // print information about process and OS
	void printPrompt() const;
	unsigned int getPromptLength() const;
	void clearPrompt() const;
	void processNewClient();
	void startConsole();
	void checkLogin() const;
	void terminateChild() const;
	void cleanExit();
	std::string getClientIpAndPort() const;
	std::string getClientIp() const;
	unsigned short getClientPort() const;
	void removeUserFromDb(const std::string &) const;
	void displayHelp() const;
	void removeSessionByPid(pid_t pid) const;
	void updateActiveUsers();
	void listActiveUsers();
	void printLineFromLog() const;
	void kickClient(const std::string &cmd);

	static const unsigned short MESSAGE_LENGTH{ 1024 };
	const std::string TEMP_DIR { "/tmp/chat_server" };
	const std::string CONFIG_FILE{ "server.cfg" };
	const std::string PROMPT{ "server>" };
	//const std::string USERLIST_LOCK{ TEMP_DIR + "/userlist.lock" };
	const int BACKLOG{ 5 };

#if defined(_WIN64) or defined(_WIN32)
	std::string getLiteralOSName(OSVERSIONINFOEX &osv) const; // Get literal version, i.e. 5.0 is Windows 2000
#endif

	std::map<std::string, ChatUser> users_;
	std::vector<std::shared_ptr<ChatMessage>> messages_;
	std::set<std::string> activeUsers_;
	std::string loggedUser_;
	ConfigFile config_{ CONFIG_FILE };
	sockaddr_in server_;
	sockaddr_in client_;
	int sockFd_;
	pid_t mainPid_;
	pid_t consolePid_;
	std::set<pid_t> children_;
	mutable char message_[MESSAGE_LENGTH];
	std::atomic_bool mainLoopActive_{ true };
	std::unique_ptr<Logger> logger_;
	int connection_;
	Mysql mysql_;
};

