#include "../inc/Server.hpp"
#include <sys/socket.h>

Server::Server(const int port, const std::string& password) 
	: _port(port), _password(password) {}

bool Server::isValid() const {
	return (_port >= MIN_PORT && _port <= MAX_PORT) && !_password.empty();
}

void Server::closeFds() { }

int Server::createSocket() {
    int sFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sFd == -1) {
        throw (std::runtime_error("Failed to create socket: "));
    }
    return sFd;
}

// gets the current flags of the file descriptor and sets them to non blocking
int Server::setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        return (-1);
    }

    flags |= O_NONBLOCK;

    int ret = fcntl(fd, F_SETFL, flags);
    if (ret == -1) {
        return (-1);
    }

    return (0);
}

void Server::bindSocket(int sFd) {
    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(this->_port);

    if (bind(sFd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
        throw std::runtime_error("Failed to bind socket: " + std::string(strerror(errno)));
    }
}


void Server::startListening(int sFd) {
    if (listen(sFd, SOMAXCONN) == -1) {
        throw (std::runtime_error("Failed to start listening on socket: "));
    }
}

// void Server::handleNewConnection(int sFd, std::vector<pollfd>& fds) {
//     int clientFd = accept(sFd, NULL, NULL);  // Replace NULL with proper sockaddr_in
//     if (clientFd == -1) {
//         throw std::runtime_error("Failed to accept new connection: " + std::string(strerror(errno)));
//     }

//     // Set the client socket to non-blocking
//     if (setNonBlocking(clientFd) == -1) {
//         throw std::runtime_error("Failed to set non-blocking on client socket: " + std::string(strerror(errno)));
//     }

//     // Add new client to the poll list
//     pollfd clientPoll;
//     clientPoll.fd = clientFd;
//     clientPoll.events = POLLIN;
//     clientPoll.revents = 0;
//     fds.push_back(clientPoll);

//     // Insert client into the map (handling exception if needed)
//     try {
//         connectedClients.emplace(clientFd, Client(clientFd));
//     } catch (const std::exception& e) {
//         throw std::runtime_error("Failed to store new client: " + std::string(e.what()));
//     }

//     // Send a welcome message
//     const std::string welcome = ":ircserv 001 client :Welcome to ft_irc\r\n";
//     send(clientFd, welcome.c_str(), welcome.length(), 0);

//     std::cout << "New client connected, fd: " << clientFd << "\n";
// }

void Server::handleNewConnection(int sFd, std::vector<pollfd>& fds) {
    sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    int clientFd = accept(sFd, (struct sockaddr*)&clientAddr, &addrLen);
    if (clientFd == -1) {
        throw std::runtime_error("Failed to accept new connection: " + std::string(strerror(errno)));
    }

    if (setNonBlocking(clientFd) == -1) {
        throw std::runtime_error("Failed to set non-blocking mode for client socket");
    }

    pollfd clientPoll;
    clientPoll.fd = clientFd;
    clientPoll.events = POLLIN;
    clientPoll.revents = 0;
    fds.push_back(clientPoll);

	try {
		connectedClients.insert(std::make_pair(clientFd, Client(clientFd)));
	} catch (const std::exception& e) {
		throw std::runtime_error("Failed to store new client: " + std::string(e.what()));
	}

    debugPrint("New client connected, FD: " + std::to_string(clientFd));

    const std::string welcome = ":ircserv 001 client :Welcome to ft_irc\r\n";
    ssize_t bytesSent = send(clientFd, welcome.c_str(), welcome.length(), 0);
    if (bytesSent == -1) {
        throw std::runtime_error("Failed to send welcome message to client");
    }

    debugPrint("Welcome message sent to client FD: " + std::to_string(clientFd));
}

void Server::handleClientMessage(size_t clientIndex, std::vector<pollfd>& fds, char* buffer) {
	int clientFd = fds[clientIndex].fd;
    std::memset(buffer, 0, MAX_BUF);
	
    ssize_t bytesRead = recv(clientFd, buffer, MAX_BUF, 0);
    if (bytesRead <= 0) {
		if (bytesRead == 0) {
			debugPrint("Client disconnected, fd: " + std::to_string(clientFd) + "\n");
        } else {
			std::cerr << "Error reading from client fd " << clientFd << ": " << strerror(errno) << "\n";
        }
        close(clientFd);
        fds.erase(fds.begin() + clientIndex);
        connectedClients.erase(clientFd);
    } else {
		// Handle the received message here

		debugPrint("Received from client fd " + std::to_string(clientFd) + ": " + buffer + "\n");
        // Process the message
    }
}

void Server::run(int sFd) {
	std::vector<pollfd> fds;
	
    pollfd serverPoll;
    serverPoll.fd = sFd;
    serverPoll.events = POLLIN;
    serverPoll.revents = 0;
    fds.push_back(serverPoll);
	
	debugPrint("Server started on port " + std::to_string(_port) + "\n");

    char buffer[MAX_BUF];
    
    while (true) {

        int pollRet = poll(fds.data(), fds.size(), -1);
        
        if (pollRet <= 0) {
            if (pollRet == 0) {
                continue ;
            } else {
                throw (std::runtime_error("Poll failed: " + std::string(strerror(errno))));
            }
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
