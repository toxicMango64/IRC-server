#include "../inc/Server.hpp"

void trimWhitespace(std::string& inputString) {
	const size_t lastNonWhitespace = inputString.find_last_not_of(" \t\n\r");
	if (lastNonWhitespace != std::string::npos) {
		inputString.erase(lastNonWhitespace + 1);
	} else {
		inputString.clear();
	}

	const size_t firstNonWhitespace = inputString.find_first_not_of(" \t\n\r");
	if (firstNonWhitespace != std::string::npos) {
		inputString.erase(0, firstNonWhitespace);
	} else {
		inputString.clear();
	}
}

bool validatePortString(const std::string& portString) {
	if (portString.empty() || (portString.size() > 1 && portString[0] == '0')) {
		std::cerr << "Invalid port format\n";
		return (false);
	}
	return (true);
}

int64_t stringToLong(const std::string& inputString) {
	std::istringstream iss(inputString);
	int64_t	result = 0;

	iss >> result;

	return (result);
}

bool validatePort(const std::string& portStr, int& result) {
    if (portStr.empty()) {
        std::cerr << "Error: Port cannot be empty.\n";
        return false;
    }
    if (portStr.length() > 5) {
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
    if (result < 1024 || result > 65535) {
        std::cerr << "Error: Port must be between Common range for non-privileged ports (1024 - 65535).\n";
        return false;
    }
    return true;
}

bool validatePassword(const std::string& password) {
	std::string processedPassword = password;
	trimWhitespace(processedPassword);

	if (processedPassword.empty()) {
		std::cerr << "Password cannot be empty\n";
		return (false);
	}
	return (true);
}
