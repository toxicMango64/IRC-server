#include "../inc/Server.hpp"
#include <sys/socket.h>

Server::Server(const int port, const std::string& password) 
	: _port(port), _password(password) {}

bool Server::isValid() const {
	return (_port >= MIN_PORT && _port <= MAX_PORT) && !_password.empty();
}

void	Server::closeFds() {
	// for(size_t i = 0; i < clients.size(); i++){
	// 	std::cout << "Client <" << clients[i].GetFd() << "> Disconnected\n";
	// 	close(clients[i].GetFd());
	// }
	// if (sfds != -1){	
	// 	std::cout << "Server <" << sfds << "> Disconnected\n";
	// 	close(sfds);
	// }
}

// wrapper funciton for socket
int Server::createSocket() {
	int sFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sFd == -1) {
		throw (std::runtime_error("Failed to create socket: "));
    }
    return sFd;
}

// wrapper funciton for fcntl
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

// wrapper funciton for fcntl
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

void Server::handleNewConnection(int sFd, std::vector<pollfd>& fds) {
	Client cli;
    sockaddr_in clientAddr;
	std::memset(&clientAddr, 0, sizeof(clientAddr));
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

void Server::handleClientMessage(size_t clientIndex, std::vector<pollfd>& fds) {

	char buffer[MAX_BUF]; // every client gets their own buffer size
	int clientFd = fds[clientIndex].fd;
    std::memset(buffer, 0, MAX_BUF);
	
    ssize_t bytesRead = recv(clientFd, buffer, MAX_BUF, 0);
    if (bytesRead <= 0) {
		if (bytesRead == 0) {
			debugPrint("Client disconnected, fd: " + std::to_string(clientFd));
        } else {
			std::cerr << "Error reading from client fd " << clientFd << ": " << strerror(errno) << "\n";
        }
        close(clientFd);
        fds.erase(fds.begin() + clientIndex);
        connectedClients.erase(clientFd);
    } else {
		// Handle the received message here
		debugPrint("Received from client fd " + std::to_string(clientFd) + ": " + buffer);

        // // Process the message
		// buffer[bytesRead] = '\0';
		// cli->append_to_buffer(buffer);
		// if (cli->get_buffer().find_first_of(CRLF) != std::string::npos)
		// {
		// 	_execute_command(cli->get_buffer(), fd);
		// 	cli->clear_buffer();
		// }
    }
}

bool Server::Signal = false;
void Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::Signal = true;
}

void Server::run(int sFd) {
	std::vector<pollfd> fds;
	
    pollfd serverPoll;
    serverPoll.fd = sFd;
    serverPoll.events = POLLIN;
    serverPoll.revents = 0;
    fds.push_back(serverPoll);

	signal(SIGINT, Server::SignalHandler);
	signal(SIGQUIT, Server::SignalHandler);
	signal(SIGPIPE, SIG_IGN); // or MSG_NOSIGNAL flag in send() to ignore SIGPIPE on linux systems

	debugPrint("Server started on port " + std::to_string(_port));
    
    while (Server::Signal == false) { // nuha's signal addition goes here as condition

        int pollRet = poll(fds.data(), fds.size(), -1);
        
		if ( -1 == pollRet && Server::Signal == false ) {
			continue ;
		} else {
			throw (std::runtime_error("Poll failed: " + std::string(strerror(errno))));
		}

        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == sFd) {
                    handleNewConnection(sFd, fds);
                } else {
                    handleClientMessage(i, fds);
                }
            }
        }
    }
}
