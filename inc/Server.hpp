// Server.hpp
#pragma once

#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <poll.h>
#include <string>
#include <stdexcept>
#include <stdint.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include "CommandHandler.hpp"
#include "Client.hpp"
#include "Utils.hpp"

// enum class e_irc : std::uint16_t {
enum e_irc {
	MIN_PORT = 0,
	MAX_PORT = 65535,
	MAX_INT_INDEX = 10
};
class Client;

class Server {
	public:
		Server( int port, const std::string& password );
		void	handleClientMessage(size_t i, std::vector<pollfd>& fds, char* buffer);
		
		bool	isValid( void ) const;
		void	run( void );
		int		createSocket();
		void	setNonBlocking(int fd);
		void	bindSocket(int sFd);
		void	startListening(int sFd);
		void	handleNewConnection(int sFd, std::vector<pollfd>& fds);

	private:
		int _port;
		std::string _password;
		std::map<int, Client> connectedClients;
};
