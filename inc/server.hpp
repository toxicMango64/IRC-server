// server.hpp
#pragma once

#include "utils.hpp"
#include <string>
#include <iostream>
#include <stdexcept>
// #include <algorithm>

// enum class e_irc : std::uint16_t {
enum e_irc {
	MIN_PORT = 0,
	MAX_PORT = 65535,
	MAX_INT_INDEX = 10
};

class Server {
	public:
		Server(int port, const std::string& password);
		bool isValid() const;
		
	private:
		int port;
		std::string password;
};
