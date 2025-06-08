// Server.hpp
#pragma once

# include "Utils.hpp"
# include <string>
# include <iostream>
# include <stdexcept>
# include <stdint.h>
# include <vector>
# include <poll.h>
# include <fcntl.h>
# include <unistd.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <cstring>

// enum class e_irc : std::uint16_t {
enum e_irc {
	MIN_PORT = 0,
	MAX_PORT = 65535,
	MAX_INT_INDEX = 10
};

class Server {
private:
	int port;
	std::string password;

public:
	Server(int port, const std::string& password);
	bool isValid() const;
	void run();
};
