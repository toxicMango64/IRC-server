/**
 * +----------------------------------------+
 * |  _                                     |
 * | (_) _ __  ___  ___   ___  _ __ __   __ |
 * | | || '__|/ __|/ __| / _ \| '__|\ \ / / |
 * | | || |  | (__ \__ \|  __/| |    \ V /  |
 * | |_||_|   \___||___/ \___||_|     \_/   |
 * +----------------------------------------+
*/

#include "../inc/server.hpp"
#include "../inc/server_utils.hpp"
#include "server.hpp"
#include <iostream>

int main(int ac, char** av) {
	if (ac != 3) {
		std::cerr << "Usage: " << av[0] << " <port> <password>\n";
		return 1;
	}

	int port;
	if (!validatePort(av[1], port)) {
		return 1;
	}

	if (!validatePassword(av[2])) {
		return 1;
	}

	Server server(port, av[2]);
	std::cout << "Server started on port " << port << "\n";
	
	return 0;
}
