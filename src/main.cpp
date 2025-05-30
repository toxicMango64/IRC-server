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

// too confusing for others to read
// 	return (ac != 3) ? (std::cerr << "Usage: " << av[0] << " <port> <password>\n", 1) :
// 		   (!validatePort(av[1], port = int())) ? 1 :
// 		   (!validatePassword(av[2])) ? 1 :
// 		   (Server(port, av[2]), std::cout << "Server started on port " << port << "\n", 0);

int main(const int ac, const char * const * av) {

	int port;
	return (ac != 3) ? (std::cerr << "Usage: " << av[0] << " <port> <password>\n", 1) :
			(!validatePort(av[1], port = int())) ? 1 :
			(!validatePassword(av[2])) ? 1 :
			(Server(port, av[2]), std::cout << "Server started on port " << port << "\n", 0);

			// if (3 != ac) {
		// 	std::cerr << "Usage: " << av[0] << " <port> <password>\n";
		// 	return (1);
		// }
		
	// int port;
	// if (!validatePort(av[1], port) || !validatePassword(av[2])) {
	// 	return (1);
	// }

	// Server server(port, av[2]);
	// std::cout << "[D] Server started on port " << port << "\n";

	return (0);
}
