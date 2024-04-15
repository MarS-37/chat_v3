#pragma once

#include "config_file.h"
#include "logger.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>

#if defined(_WIN64) or defined(_WIN32)
#include <Windows.h>
struct WindowsVersion {
	unsigned major, minor, build;
};
#elif defined(__linux__)
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#endif

class ChatClient final {
public:
	ChatClient(); // constructor
	~ChatClient(); // destructor
	void work(); // main work
	void sigIntHandler(int signum) const;
	void sigTermHandler(int signum) const;

private:
	bool isLoginAvailable(const std::string& login) const; // login availability
	void signUp(); // registration
	bool isValidLogin(const std::string& login) const; // login verification
	void signIn(); // authorization
	void signOut(); // user logout
	void removeUser(); // deleting a user
	ssize_t sendRequest() const; // sending a message
	ssize_t receiveResponse() const; // receiving a response
	void sendPrivateMessage(const std::string &senderName, const std::string& receiverName, const std::string& messageText); // sending a private message
	void sendBroadcastMessage(const std::string &senderName, const std::string& message); // sending a shared message
	void printSystemInformation() const; // print information about process and OS
	void printPrompt() const;
	void writeResponseToFile() const;
	bool readResponseFromFile() const;
	void startPoller();
	void clearPrompt() const;
	void cleanExit() const;
	void displayHelp() const;
	
	static const unsigned short MESSAGE_LENGTH{ 1024 };
	const std::string USER_CONFIG{ "users.cfg" };
	const std::string MESSAGES_LOG{ "messages.log" };
	const std::string CONFIG_FILE{ "client.cfg" };
	const std::string TEMP_DIR{ "/tmp/chat_client" };
	const std::string RESPONSE_LOCK{ TEMP_DIR + "/response.lock" };
	const std::string RESPONSE{ TEMP_DIR + "/response.tmp" };

#if defined(_WIN64) or defined(_WIN32)
	std::string getLiteralOSName(OSVERSIONINFOEX &osv) const; // Get literal version, i.e. 5.0 is Windows 2000
#endif

	std::string loggedUser_;
	ConfigFile config_{ CONFIG_FILE };
	sockaddr_in server_;
	pid_t mainPid_;
	pid_t pollerPid_;
	int sockFd_;
	std::unique_ptr<Logger> logger_;
	mutable char message_[MESSAGE_LENGTH];
};
