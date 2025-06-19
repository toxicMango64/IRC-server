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
	
	// Signal handling is now managed within Server::run()
	// The Server::SignalHandler sets a flag for graceful shutdown.

	try {
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
