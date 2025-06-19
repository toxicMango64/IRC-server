#include <csignal>
#include <netinet/tcp.h> // For SO_LINGER
#include <stdexcept>
#include <stdlib.h>
#include <sys/socket.h>

#include "Response.hpp"
#include "Server.hpp"

Server::Server() { }
Server::~Server( ) { }
Server::Server( Server const &src ) { *this = src; }
Server &Server::operator=( Server const &src ) {
	if (this != &src) {
		this->_port = src._port;
		this->sfds = src.sfds;
		this->_password = src._password;
		this->clients = src.clients;
		this->channels = src.channels;
		this->fds = src.fds;
	}
	return (*this);
}

Server::Server(const int port, const std::string& password) 
	: _port(port), _password(password) {
	int sFd = createSocket();

	setNonBlocking(sFd);
	bindSocket(sFd);
	startListening(sFd);
	run(sFd);
}

bool Server::_signalRecvd = false;

void Server::signalHandler(int signum) {
	if (signum == SIGINT) {
		std::cerr << "\nCaught SIGINT (Ctrl+C). Stopping server...\n";
	} 
	else if (signum == SIGQUIT) {
		std::cerr << "\nCaught SIGQUIT (Ctrl+D). Stopping server...\n";
	}
    else {
        // Handle any unexpected signal
        std::cerr << "\nCaught unexpected signal. Stopping server...\n";
    }
    _signalRecvd = true;
    // exit(1);
}

bool Server::isValid() const {
	return (_port >= MIN_PORT && _port <= MAX_PORT) && !_password.empty();
}

void Server::closeFds() {
    struct linger linger_opt = { .l_onoff = 1, .l_linger = 0 }; // Hard close

    // Close clients
    for (size_t i = 0; i < clients.size(); i++) {
        setsockopt(clients[i].GetFd(), SOL_SOCKET, SO_LINGER, &linger_opt, sizeof(linger_opt));
        close(clients[i].GetFd());
        std::cout << "Client <" << clients[i].GetFd() << "> Disconnected \n";
    }

    // Close server socket
    if (sfds != -1) {
        std::cout << "Server <" << sfds << "> Disconnected \n";
		close(sfds);
    }
}

// wrapper funciton for socket
int Server::createSocket() {
	int sFd = socket(AF_INET, SOCK_STREAM, 0);
	if (sFd == -1) {
		throw (std::runtime_error("Failed to create socket: "));
	}
	SetFd(sFd);
	return sFd;
}

// wrapper funciton for fcntl
// gets the current flags of the file descriptor and sets them to non blocking
int Server::setNonBlocking(int fd) {
	return (fcntl(fd, F_SETFL, O_NONBLOCK));
}

// wrapper funciton for fcntl
void Server::bindSocket(int sFd) {
	sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(this->_port);

    int yes = 1;
    if (setsockopt(sFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        throw std::runtime_error("Failed to set SO_REUSEADDR");
    }
	if (bind(sFd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
		throw std::runtime_error("Failed to bind socket:");
	}
}

void Server::startListening(int sFd) {
	if (listen(sFd, SOMAXCONN) == -1) {
		throw (std::runtime_error("Failed to start listening on socket: "));
	}
}

void Server::handleNewConnection(int sFd, std::vector<pollfd>& fds) {
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
        AddClient(Client(clientFd));
	} catch (const std::exception& e) {
		throw std::runtime_error("Failed to store new client: " + std::string(e.what()));
	}

	logMsg("New client connected, FD: {%i}", clientFd);

}

void Server::handleClientMessage(size_t clientIndex, std::vector<pollfd>& fds) {

	char buffer[MAX_BUF]; // every client gets their own buffer size
	int clientFd = fds[clientIndex].fd;
	std::memset(buffer, 0, MAX_BUF);
	
	ssize_t bytesRead = recv(clientFd, buffer, MAX_BUF, 0);
	if (bytesRead <= 0) {
		if (bytesRead == 0) {
			logMsg("Client disconnected, fd: {%i}", clientFd);
			RmChannels(clientFd);
			RemoveClient(clientFd);
			RemoveFds(clientFd);
		} else {
			std::cerr << "Error reading from client fd " << clientFd << ": " << strerror(errno) << "\n";
		}
		close(clientFd);
		fds.erase(fds.begin() + clientIndex);
		connectedClients.erase(clientFd);
	} else {
		// Handle the received message here
		logMsg("Received from client fd: {%i} : {%s}", clientFd, buffer);

		// Process the message
		Client *client = GetClient(clientFd);

        if (!client) {
            logMsg("Client was not found or something");
            return ;
        }
		client->setBuffer(buffer);
		if (client->getBuffer().find_first_of("\r\n") == std::string::npos)
			return ;

		std::vector<std::string> cmd;
		cmd = splitReceivedBuffer(client->getBuffer());
		for (size_t i = 0; i < cmd.size(); i++) {
			this->getCmd(cmd[i], clientFd);
		}
		if (GetClient(clientFd)) {
			GetClient(clientFd)->clearBuffer();
		}
	}
}

void Server::run(int sFd) {
	std::vector<pollfd> fds;
	
	pollfd serverPoll;
	serverPoll.fd = sFd;
	serverPoll.events = POLLIN;
	serverPoll.revents = 0;
	fds.push_back(serverPoll);

	signal(SIGINT, Server::signalHandler);
	signal(SIGQUIT, Server::signalHandler);
	signal(SIGPIPE, SIG_IGN); // or MSG_NOSIGNAL flag in send() to ignore SIGPIPE on linux systems

	logMsg("Server started on port: {%i}", _port);

	while (Server::_signalRecvd == false) {

		int pollRet = poll(fds.data(), fds.size(), -1);
		
		if (pollRet == -1) {
            if (errno == EINTR && Server::_signalRecvd) {
                break;
            }
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
    closeFds();
}

/** getters */
int Server::GetPort(){return this->_port;}
int Server::GetFd(){return this->sfds;}
Client *Server::GetClient(int fd){
	for (size_t i = 0; i < this->clients.size(); i++){
		if (this->clients[i].GetFd() == fd)
			return &this->clients[i];
	}
	return NULL;
}

Client *Server::GetClientNick(std::string nickname){
	for (size_t i = 0; i < this->clients.size(); i++){
		if (this->clients[i].GetNickName() == nickname)
			return &this->clients[i];
	}
	return NULL;
}

Channel *Server::GetChannel(std::string name)
{
	for (size_t i = 0; i < this->channels.size(); i++){
		if (this->channels[i].GetName() == name)
			return &channels[i];
	}
	return NULL;
}

/** setters */
void Server::SetFd( int fd ) { this->sfds = fd; }
void Server::SetPort( int port ) { this->_port = port; }
void Server::SetPassword( const std::string password ) { this->_password = password; }

std::string Server::GetPassword(  ) { return this->_password; }

void Server::AddClient( Client newClient ) { this->clients.push_back( newClient); }
void Server::AddChannel( Channel newChannel ) { this->channels.push_back( newChannel); }
void Server::AddFds( pollfd newFd ) { this->fds.push_back(newFd); }

void Server::RemoveClient(int fd){
	for (size_t i = 0; i < this->clients.size(); i++){
		if (this->clients[i].GetFd() == fd)
			{this->clients.erase(this->clients.begin() + i); return;}
	}
}
void Server::RemoveChannel(std::string name){
	for (size_t i = 0; i < this->channels.size(); i++){
		if (this->channels[i].GetName() == name)
			{this->channels.erase(this->channels.begin() + i); return;}
	}
}
void Server::RemoveFds(int fd){
	for (size_t i = 0; i < this->fds.size(); i++){
		if (this->fds[i].fd == fd)
			{this->fds.erase(this->fds.begin() + i); return;}
	}
}

void	Server::RmChannels(int fd){
	for (size_t i = 0; i < this->channels.size(); i++){
		int flag = 0;
		if (channels[i].get_client(fd))
			{channels[i].remove_client(fd); flag = 1;}
		else if (channels[i].get_admin(fd))
			{channels[i].remove_admin(fd); flag = 1;}
		if (channels[i].GetClientsNumber() == 0)
			{channels.erase(channels.begin() + i); i--; continue;}
		if (flag){
			std::string rpl = ":" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + "@server QUIT Quit\r\n";
			channels[i].sendTo_all(rpl);
		}
	}
}

void Server::senderror(int code, std::string clientname, int fd, std::string msg)
{
	std::stringstream ss;
	ss << ":server " << code << " " << clientname << msg;
	std::string resp = ss.str();
	if(send(fd, resp.c_str(), resp.size(),0) == -1)
		std::cerr << "send() faild \n";
}

void Server::senderror(int code, std::string clientname, std::string channelname, int fd, std::string msg)
{
	std::stringstream ss;
	ss << ":server " << code << " " << clientname << " " << channelname << msg;
	std::string resp = ss.str();
	if(send(fd, resp.c_str(), resp.size(),0) == -1)
		std::cerr << "send() faild \n";
}

// add carnage returns and new line in the end manually
void Server::_sendResponse(std::string response, int fd)
{
	if(send(fd, response.c_str(), response.size(), 0) == -1)
		std::cerr << "Response send() faild \n";
}

void Server::getCmd(std::string& cmd, int fd)
{
	if (cmd.empty())
		return;

	const size_t firstNonSpace = cmd.find_first_not_of(" \t\v");
	if (firstNonSpace != std::string::npos)
		cmd.erase(0, firstNonSpace);

	std::vector<std::string> tokens = splitCmd(cmd);
	if (tokens.empty()) {
		return ;
	}

	std::string command = tokens[0];
	// for (size_t i = 0; i < command.length(); ++i) {
	// 	command[i] = static_cast<char>(std::tolower(command[i]));
	// }

	if (command == "PING")
		_sendResponse("PONG\r\n", fd);
    else if (command == "CAP"){
        // std::cout << "Capability negotiation in progress" << std::endl;
        _sendResponse("CAP * LS :\r\n", fd);
    }
	else if (command == "PASS")
		client_authen(fd, cmd);
	else if (command == "NICK")
		set_nickname(cmd, fd);
	else if (command == "USER")
		set_username(cmd, fd);
	else if (command == "QUIT")
		QUIT(cmd, fd);
	else if (GetClient(fd)->getRegistered()) {
		if (command == "KICK")
			KICK(cmd, fd);
		else if (command == "JOIN") {
			logMsg("the thing that happened cmd: {%s} fd: {%i}", cmd.c_str(), fd);
			JOIN(cmd, fd);
		}
		else if (command == "TOPIC")
			Topic(cmd, fd);
		else if (command == "MODE")
			mode_command(cmd, fd);
		else if (command == "PART")
			PART(cmd, fd);
		else if (command == "PRIVMSG")
			PRIVMSG(cmd, fd);
		else if (command == "INVITE")
			Invite(cmd, fd);
		else
			_sendResponse(ERR_CMDNOTFOUND(GetClient(fd)->GetNickName(), tokens[0]), fd);
	} else {
		_sendResponse(ERR_NOTREGISTERED("*"), fd);
	}
}
