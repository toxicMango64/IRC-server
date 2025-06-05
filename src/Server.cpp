#include "Server.hpp"
#include "Utils.hpp"

Server::Server(int port, const std::string& password) 
	: _port(port), _password(password) {}

bool Server::isValid(void) const {
	return (_port >= MIN_PORT && _port <= MAX_PORT) && !_password.empty();
}

const char* Server::SocketCreationException::what() const throw() {
    return ("Couldn't create a socket");
}

const char* Server::SocketBindingException::what() const throw() {
    return ("Couldn't bind socket");
}

const char* Server::SocketListeningException::what() const throw() {
    return ("Couldn't listen on socket");
}

const char* Server::PollException::what() const throw() {
    return ("poll() failed");
}

void    Server::run(void) {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw SocketCreationException();
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(_port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        close(serverSocket);
        throw SocketBindingException();
    }
    if (listen(serverSocket, 5) < 0) {
        close(serverSocket);
        throw SocketListeningException();
    }

    std::vector<pollfd> pollfds;
    pollfd serverPollfd = {serverSocket, POLLIN, 0};
    pollfds.push_back(serverPollfd);

    std::cout << "Server started on port " << _port << std::endl;

    while (true) {
        int pollCount = poll(pollfds.data(), pollfds.size(), -1);
        if (pollCount < 0) {
            throw PollException();
        }

        if (pollfds[0].revents & POLLIN) {
            sockaddr_in clientAddress;
            socklen_t clientLen = sizeof(clientAddress);
            int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientLen);
            if (clientSocket >= 0) {
                pollfd clientPollfd = {clientSocket, POLLIN, 0};
                pollfds.push_back(clientPollfd);
                std::cout << "New client connected: " << clientSocket << std::endl;
            }
        }
        for (size_t i = 1; i < pollfds.size(); ++i) {
            if (pollfds[i].revents & POLLIN) {
                char buffer[512];
                ssize_t bytesRead = recv(pollfds[i].fd, buffer, sizeof(buffer) - 1, 0);
                if (bytesRead > 0) {
                    buffer[bytesRead] = '\0';
                    std::cout << "Received from client " << pollfds[i].fd << ": " << buffer << std::endl;
                    // Parse and handle IRC commands here
                } else {
                    std::cout << "Client disconnected: " << pollfds[i].fd << std::endl;
                    close(pollfds[i].fd);
                    pollfds.erase(pollfds.begin() + i);
                    --i;
                }
            }
        }
    }
    for (size_t i = 0; i < pollfds.size(); ++i) {
        close(pollfds[i].fd);
    }
}
