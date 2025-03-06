#include "../inc/server.hpp"
#include "../inc/server_utils.hpp"

Server::Server(int port, const std::string& password) 
	: port(port), password(password) {}

bool Server::isValid() const {
	return (port >= MIN_PORT && port <= MAX_PORT) && !password.empty();
}
