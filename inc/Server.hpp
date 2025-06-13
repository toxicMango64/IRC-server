// Server.hpp
#pragma once

#include <stdexcept>
#include <stdint.h>
#include <iostream>
#include "Utils.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <map>
#include <netinet/in.h>
#include <poll.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include "CommandHandler.hpp"
#include "Client.hpp"

// enum class e_irc : std::uint16_t {
enum e_irc {
	MIN_PORT = 0,
	MAX_PORT = 65535,
	MAX_INT_INDEX = 10
};

#include <stdio.h>

#define welcome(server) printf("welcome to {%s}, hope you enjoy your time here\n", server);

class Client;

class Server {
	public:
		static const int MAX_BUF = 512;
		Server( int port, const std::string& password );
		void	handleClientMessage(size_t i, std::vector<pollfd>& fds, char* buffer);
		
		bool	isValid( void ) const;
		void	run( int sFd );
		int		createSocket( void );
		void	setNonBlocking( int fd );
		void	bindSocket( int sFd );
		void	startListening( int sFd );
		void	handleNewConnection( int sFd, std::vector<pollfd>& fds );

		const int& getPort() const { return _port; }
		const std::string& getPassword() const { return _password; }

	private:
		int _port;
		std::string _password;
		std::map<int, Client> connectedClients;
};
