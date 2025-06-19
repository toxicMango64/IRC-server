#include "../inc/Utils.hpp"
#include <algorithm> // For std::all_of
#include <cctype>    // For std::isdigit
#include <sstream>
#include <vector>

bool validatePort(const std::string& portStr, int& result) {
    if (portStr.empty()) {
        std::cerr << "Error: Port cannot be empty.\n";
        return false;
    }
    if (portStr.length() > 5) { // Max port is 65535, 5 digits
        std::cerr << "Error: Port number too long.\n";
        return false;
    }
    for (size_t i = 0; i < portStr.length(); ++i) {
        if (!std::isdigit(portStr[i])) {
            std::cerr << "Error: Port must be a number.\n";
            return false;
        }
    }
    std::stringstream ss(portStr);
    ss >> result;
    if (ss.fail() || !ss.eof()) {
        std::cerr << "Error: Invalid port number format.\n";
        return false;
    }
    if (result < 1024 || result > 65535) { // Common range for non-privileged ports
        std::cerr << "Error: Port must be between 1024 and 65535.\n";
        return false;
    }
    return true;
}

bool validatePassword(const std::string& password) {
    if (password.empty()) {
        std::cerr << "Error: Password cannot be empty.\n";
        return false;
    }
    // Add more password validation rules if necessary (e.g., min length, complexity)
    return true;
}

bool isValidNickname(const std::string& nickname) {
    if (nickname.empty() || nickname.length() > 9) {
        return false;
    }
    // Nicknames must start with a letter and can contain letters, numbers, and specific symbols, alphanumeric in short
    if (!std::isalpha(nickname[0])) {
        return false;
    }
    for (size_t i = 1; i < nickname.length(); ++i) {
        if (!std::isalnum(nickname[i]) && nickname[i] != '-' && nickname[i] != '[' && nickname[i] != ']' && nickname[i] != '\\' && nickname[i] != '`' && nickname[i] != '^' && nickname[i] != '{' && nickname[i] != '}') {
            return false;
        }
    }
    return true;
}

std::vector<std::string> splitReceivedBuffer(const std::string& str) {
	std::vector<std::string> vec;
	std::istringstream stm(str);
	std::string line;
	while (std::getline(stm, line)) {
		const size_t pos = line.find_first_of("\r\n");
		if (pos != std::string::npos)
			line = line.substr(0, pos);
		vec.push_back(line);
	}
	return vec;
}
