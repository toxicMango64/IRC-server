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

/**
 * Checks: 
 * - `ac != 3`: Prints usage message and returns 1.
 * - `!validatePort(av[1], port)`: Returns 1 if port is invalid.
 * - `!validatePassword(av[2])`: Returns 1 if password is invalid.
 * - Else: Initializes and runs server inside a lambda, returns 0 on success or 1 on exception.
 */
int main(const int ac, const char *const *av) {
	
	if (ac != 3) {
		std::cerr << "Usage: " << av[0] << " <port> <password>\n";
		return 1;
	}
	
	int	port = 0;
	if (!validatePort(av[1], port) || (!validatePassword(av[2]))) {
		return (1);
	}
	
	try {
		std::cout << "Server started on port " << port << "\n";
		const Server server(port, av[2]);
		// server.run();
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		return (1);
	}
	
	return (0);
}

// return (ac != 3) ? (std::cerr << "Usage: " << av[0] << " <port> <password>\n", 1) :
// 	(!validatePort(av[1], port)) ? 1 :
// 	(!validatePassword(av[2])) ? 1 :
// 	([&]() -> int {
// 		try {
// 			Server server(port, av[2]);
// 			std::cout << "Server started on port " << port << "\n";
// 			// server.run();
// 			return 0;
// 		} catch (const std::exception& e) {
// 			std::cerr << "Error: " << e.what() << std::endl;
// 			return 1;
// 		}
// 	})();
