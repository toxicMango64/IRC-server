// server.hpp
#pragma once

#include <exception>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <poll.h>
#include <stdexcept>
#include <string>
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

class Server {
    public:
        Server( int port, const std::string& password );
        bool isValid( void ) const;
    	void run( void );
        std::map<int, Client> connectedClients;
    
    // Exception classes
    public:
        class SocketCreationException: public std::exception {
            public:
                virtual const char* what() const throw();
        };
        class SocketBindingException: public std::exception {
            public:
                virtual const char* what() const throw();
        };
        class SocketListeningException: public std::exception {
            public:
                virtual const char* what() const throw();
        };
        class PollException: public std::exception {
            public:
                virtual const char* what() const throw();
        };

    private:
        int _port;
    	std::string _password;
};
