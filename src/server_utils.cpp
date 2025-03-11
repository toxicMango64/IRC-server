#include "../inc/server.hpp"
#include "server_utils.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>

void trimWhitespace(std::string& str) {
	// Trim trailing whitespace
	size_t last = str.find_last_not_of(" \t\n\r");
	if (last != std::string::npos) {
		str.erase(last + 1);
	} else {
		str.clear(); // Entire string was whitespace
	}

	// Trim leading whitespace
	size_t first = str.find_first_not_of(" \t\n\r");
	if (first != std::string::npos) {
		str.erase(0, first);
	} else {
		str.clear(); // Entire string was whitespace
	}
}

bool validatePortString(const std::string& portStr) {
	if (portStr.empty() || (portStr.size() > 1 && portStr[0] == '0')) {
		std::cerr << "Invalid port format\n";
		return (false);
	}
	return (true);
}

#include <iostream>
#include <sstream>
#include <string>

long stringToLong(const std::string& str) {
	std::istringstream iss(str);
	long result;
	iss >> result; // Extracts the long value from the string stream
	return result;
}

bool validatePort(const std::string& portStr, int& result) {
	std::string processed = portStr;
	trimWhitespace(processed);
	
	if (!validatePortString(processed)) return (false);

	try {
		long port = stringToLong(processed);
		if (port < MIN_PORT || port > MAX_PORT) {
			std::cerr << "Port out of range (" 
					<< MIN_PORT << "-" << MAX_PORT << ")\n";
			return (false);
		}
		result = static_cast<int>(port);
		return (true);
	}
	catch (const std::invalid_argument&) {
		std::cerr << "Invalid port number\n";
	}
	catch (const std::out_of_range&) {
		std::cerr << "Port value too large\n";
	}
	return (false);
}

bool validatePassword(const std::string& password) {
	std::string processed = password;
	trimWhitespace(processed);
	
	if (processed.empty()) {
		std::cerr << "Password cannot be empty\n";
		return (false);
	}
	return (true);
}
