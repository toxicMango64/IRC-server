// Server.hpp
#pragma once

#include <stdexcept>
#include <stdint.h>
#include <iostream>
#include "Utils.hpp"

#if defined(_MSC_VER)
  #ifdef BUILDING_MYLIB
    #define EXPORT __declspec(dllexport)
  #else
    #define EXPORT __declspec(dllimport)
  #endif
#elif defined(__GNUC__) || defined(__clang__)
  #define EXPORT __attribute__((visibility("default")))
#else
  #define EXPORT
#endif

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

// #include <stdio.h>

// #define welcome(server) printf("welcome to {%s}, hope you enjoy your time here\n", server);

class Client;
class Channel;

class Server {
	public:
		static const int	MAX_BUF = 512;
		static const char	buffer[MAX_BUF] __attribute__((aligned(16)));

		bool				isValid( void ) const;
		static void			signalHandler(int signum);

		Server();
		~Server();
		Server( Server const &src );
		Server &operator=( Server const &src );
		Server( int port, const std::string& password );

		void	run( int sFd );
		void	closeFds( void ); /** implement the fucntion */
		int		createSocket( void );
		void	bindSocket( int sFd );
		int		setNonBlocking( int fd );
		void	startListening( int sFd );
		void	handleClientMessage(size_t i, std::vector<pollfd>& fds);
		void	handleNewConnection( int sFd, std::vector<pollfd>& fds );

		const int& getPort() const { return _port; }
		const std::string& getPassword() const { return _password; }
		static void SignalHandler( int signum );

	private:
		// static const int MAX_BUF = 512;
		const int _port;
		const std::string _password;
		static bool Signal;

		std::map<int, Client> connectedClients;
		int server_fdsocket;
		std::vector<Client> clients;
		std::vector<Channel> channels;
		std::vector<struct pollfd> fds;
		struct sockaddr_in add;
		struct sockaddr_in cliaddr;
		struct pollfd new_cli;
};
