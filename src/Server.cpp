#include "../inc/Server.hpp"

Server::Server(int port, const std::string& password) 
	: _port(port), _password(password) {}

bool Server::isValid() const {
	return (_port >= MIN_PORT && _port <= MAX_PORT) && !_password.empty();
}

void Server::run() {
	int sFd = socket(AF_INET, SOCK_STREAM, 0);
	if (sFd < 0)
		throw std::runtime_error("Failed to create socket");

	// Make server socket non-blocking
	if (fcntl(sFd, F_SETFL, fcntl(sFd, F_GETFL) | O_NONBLOCK) < 0)
		throw std::runtime_error("Failed to set non-blocking");

	// Bind server socket
	struct sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(this->_port);

	const struct sockaddr *sockAddr = reinterpret_cast<struct sockaddr*>(&addr);
	if (bind(sFd, sockAddr, sizeof(addr)) < 0)
		throw std::runtime_error("Bind failed");

	if (listen(sFd, SOMAXCONN) < 0)
		throw std::runtime_error("Listen failed");

	std::vector<pollfd> fds;
	pollfd server_poll;
	server_poll.fd = sFd;
	server_poll.events = POLLIN;
	server_poll.revents = 0;
	fds.push_back(server_poll);

	char buffer[512];

	while (true) {
		int poll_count = poll(&fds[0], fds.size(), -1);
		if (poll_count < 0)
			throw std::runtime_error("Poll failed");

		for (size_t i = 0; i < fds.size(); ++i) {
			if (fds[i].revents & POLLIN) {
				if (fds[i].fd == sFd) {
					int client_fd = accept(sFd, NULL, NULL);
					if (client_fd > 0) {
						fcntl(client_fd, F_SETFL, fcntl(client_fd, F_GETFL) | O_NONBLOCK);
						pollfd client_poll;
						client_poll.fd = client_fd;
						client_poll.events = POLLIN;
						client_poll.revents = 0;
						fds.push_back(client_poll);
                        connectedClients.insert(std::make_pair(client_fd, Client(client_fd)));
						debugPrint("New client connected.");
						const std::string welcome = ":ircserv 001 client :Welcome to ft_irc\r\n";
						send(client_fd, welcome.c_str(), welcome.length(), 0);
					}
				} else {
					int n = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
					if (n <= 0) {
						close(fds[i].fd);
						fds.erase(fds.begin() + i);
                        connectedClients.erase(fds[i].fd);
						--i;
					} else {
                        std::string output;
						buffer[n] = '\0';
						std::cout << ">> " << buffer;
                        handleBuffer(connectedClients.at(fds[i].fd), buffer, output);
                        if (!output.empty()){
                            send(fds[i].fd, output.c_str(), output.length(), 0);
                        }
					}
				}
			}
		}
	}
}
