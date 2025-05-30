#include "../inc/Server.hpp"

/**
 * @brief Trims trailing/leading whitespace
 * @param inputString 
 */
void trimWhitespace(std::string& inputString) {
	size_t lastNonWhitespace = inputString.find_last_not_of(" \t\n\r");
	if (lastNonWhitespace != std::string::npos) {
		inputString.erase(lastNonWhitespace + 1);
	} else {
		inputString.clear();
	}

	size_t firstNonWhitespace = inputString.find_first_not_of(" \t\n\r");
	if (firstNonWhitespace != std::string::npos) {
		inputString.erase(0, firstNonWhitespace);
	} else {
		inputString.clear();
	}
}

/**
 * @brief 
 * @param portString 
 * @return 
 */
bool validatePortString(const std::string& portString) {
	if (portString.empty() || (portString.size() > 1 && portString[0] == '0')) {
		std::cerr << "Invalid port format\n";
		return (false);
	}
	return (true);
}

long stringToLong(const std::string& inputString) {
	std::istringstream iss(inputString);
	long result;
	iss >> result;
	return result;
}

bool validatePort(const std::string& portString, int& portNumber) {
	std::string processedPortString = portString;
	trimWhitespace(processedPortString);

	if (!validatePortString(processedPortString)) return (false);

	try {
		long portValue = stringToLong(processedPortString);
		if (portValue < MIN_PORT || portValue > MAX_PORT) {
			std::cerr << "Port out of range ("
					<< MIN_PORT << "-" << MAX_PORT << ")\n";
			return (false);
		}
		portNumber = static_cast<int>(portValue);
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
	std::string processedPassword = password;
	trimWhitespace(processedPassword);

	if (processedPassword.empty()) {
		std::cerr << "Password cannot be empty\n";
		return (false);
	}
	return (true);
}
