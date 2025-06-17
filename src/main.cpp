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

#include "../inc/Server.hpp"
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <iostream>

__attribute__((__noreturn__)) void Server::signalHandler(int signum) {
	if (signum == SIGINT) {
		std::cerr << "\nCaught SIGINT (Ctrl+C). Stopping server...\n";
		exit(0);
	} 
	else if (signum == SIGQUIT) {
		std::cerr << "\nCaught SIGQUIT (Ctrl+D). Stopping server...\n";
		exit(0);
	}
	// Handle any unexpected signal
	std::cerr << "\nCaught unexpected signal. Stopping server...\n";
	exit(1);
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
	act.sa_handler = Server::signalHandler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

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
		logMsg("Exception caught: %s", e.what());
		server.closeFds();
		return (1);
	} catch (const int& errnum) {
		logMsg("Caught error code: %d (%s)", errnum, std::strerror(errnum));
		server.closeFds();
		return (errnum);
	}

	return (0);
}
