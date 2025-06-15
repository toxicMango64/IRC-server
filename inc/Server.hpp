// Server.hpp
#pragma once

#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <map>
#include <netinet/in.h>
#include <poll.h>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include "Client.hpp"
#include "CommandHandler.hpp"
#include "Utils.hpp"

// enum class e_irc : std::uint16_t {
enum e_irc {
	MIN_PORT = 0,
	MAX_PORT = 65535,
	MAX_INT_INDEX = 10
};

// #include <stdio.h>

// #define welcome(server) printf("welcome to {%s}, hope you enjoy your time here\n", server);

class Client;

class Server {
	public:
		static const int MAX_BUF = 512;
		Server( int port, const std::string& password );
		bool	isValid( void ) const;
		static void SignalHandler(int signum);

		void	run( int sFd );
		void	closeFds( void ); // implement the fucntion
		int		createSocket( void );
		void	bindSocket( int sFd );
		int		setNonBlocking( int fd );
		void	startListening( int sFd );
		void	handleClientMessage(size_t i, std::vector<pollfd>& fds);
		void	handleNewConnection( int sFd, std::vector<pollfd>& fds );

		const int& getPort() const { return _port; }
		const std::string& getPassword() const { return _password; }

	private:
		const int _port;
		const std::string _password;

		std::map<int, Client> connectedClients;
};
