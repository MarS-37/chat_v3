#include "chat_server.h"


int main() {
	try {
		static ChatServer chat;
		signal(SIGTERM, [](int signum) { chat.sigTermHandler(signum); });
		signal(SIGCHLD, [](int signum) { chat.childDeathHandler(signum); });
		signal(SIGINT, [](int signum) { chat.sigIntHandler(signum); });
		chat.work();
	}
	catch (const std::runtime_error &e) {
		std::cerr << "Fatal error! " << e.what() << std::endl;
	}
	catch (const std::out_of_range &e) {
		std::cerr << "Fatal error! " << getpid() << " " << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}
