/**
* +----------------------------------------+
* |  _                                     |
* | (_) _ __  ___  ___   ___  _ __ __   __ |
* | | || '__|/ __|/ __| / _ \| '__|\ \ / / |
* | | || |  | (__ \__ \|  __/| |    \ V /  |
* | |_||_|   \___||___/ \___||_|     \_/   |
* |                                        |
* |                                        |
* +----------------------------------------+
*/

#include <csignal>
#include <cstring>
#include <iostream>
#include <stdlib.h>

#include "Server.hpp"

void signalHandler(int signal) {
	if (signal == SIGINT) {
		std::cerr << "\nCaught SIGINT (Ctrl+C). Stopping server...\n";
		exit(0);
	} 
	else if (signal == SIGQUIT) {
		std::cerr << "\nCaught SIGQUIT (Ctrl+D). Stopping server...\n";
		exit(0);
	}
}

int main(const int ac, const char *const *av) {

	if (ac != 3) {
		std::cerr << "Usage: " << av[0] << " <port> <password>\n";
		return 1;
	}

	int port = 0;
	if (!validatePort(av[1], port) || (!validatePassword(av[2]))) {
		return (1);
	}

	Server server(port, av[2]);

	struct sigaction act;

	// Set up signal handler
	act.sa_handler = signalHandler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	// Register the signal handler for SIGINT (Ctrl+C) and SIGQUIT (Ctrl+D)
	if (sigaction(SIGINT, &act, NULL) == -1) {
		std::cerr << "Error setting SIGINT handler: " << std::strerror(errno) << std::endl;
		return 1;
	}
	if (sigaction(SIGQUIT, &act, NULL) == -1) {
		std::cerr << "Error setting SIGQUIT handler: " << std::strerror(errno) << std::endl;
		return 1;
	}

	try {
		// Add server signal handling here (already integrated above)
		int sFd = server.createSocket();

		server.setNonBlocking(sFd);
		server.bindSocket(sFd);
		server.startListening(sFd);
		server.run(sFd);

	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		return (server.closeFds(), 1);
	} catch (const int& errnum) {
		std::cerr << "Error: " << std::strerror(errnum) << "\n";
		return (server.closeFds(), errnum);
	}

	return (0);
}
