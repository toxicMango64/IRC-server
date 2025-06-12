#include "../inc/Server.hpp"

Server::Server(int port, const std::string& password) 
	: _port(port), _password(password) {}

bool Server::isValid() const {
	return (_port >= MIN_PORT && _port <= MAX_PORT) && !_password.empty();
}

int Server::createSocket() {
    int sFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sFd < 0) {
        throw std::runtime_error("Failed to create socket");
    }
    return sFd;
}

void Server::setNonBlocking(int fd) {
    if (fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK) < 0) {
        throw std::runtime_error("Failed to set non-blocking");
    }
}

void Server::bindSocket(int sFd) {
    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(this->_port);

    if (bind(sFd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        throw std::runtime_error("Bind failed");
    }
}

void Server::startListening(int sFd) {
    if (listen(sFd, SOMAXCONN) < 0) {
        throw std::runtime_error("Listen failed");
    }
}

void Server::handleNewConnection(int sFd, std::vector<pollfd>& fds) {
    int client_fd = accept(sFd, NULL, NULL);
    if (client_fd > 0) {
        setNonBlocking(client_fd);
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
}

void Server::handleClientMessage(size_t i, std::vector<pollfd>& fds, char* buffer) {
    int fd = fds[i].fd;
    int n = recv(fd, buffer, 511, 0);
    if (n <= 0) {
        close(fd);
        connectedClients.erase(fd);
        fds.erase(fds.begin() + static_cast<int64_t>(i));
    } else {
        buffer[n] = '\0';
        std::string output;
        std::cout << ">> " << buffer;
        handleBuffer(connectedClients.at(fd), buffer, this->getPassword(), output);
        if (!output.empty()) {
            send(fd, output.c_str(), output.length(), 0);
        }
    }
}

void Server::run() {
    int sFd = createSocket();
    setNonBlocking(sFd);
    bindSocket(sFd);
    startListening(sFd);

    std::vector<pollfd> fds;
	pollfd server_poll;
	server_poll.fd = sFd;
	server_poll.events = POLLIN;
	server_poll.revents = 0;
    fds.push_back(server_poll);
    char buffer[512];

    std::cout << "Server started on port " << _port << "\n";
    while (true) {

        if (poll(&fds[0], fds.size(), -1) < 0) {
            throw std::runtime_error("Poll failed");
        }

        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == sFd) {
                    handleNewConnection(sFd, fds);
                } else {
                    handleClientMessage(i, fds, buffer);
                }
            }
        }
    }
}
