#include "chat_client.h"

int main() {
	try {
		static ChatClient chat;
		signal(SIGTERM, [](int signum){ chat.sigTermHandler(signum); });
		signal(SIGINT, [](int signum){ chat.sigIntHandler(signum); });
		chat.work();
	}
	catch (const std::runtime_error &e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}
