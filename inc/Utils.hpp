// Utils.hpp
#pragma once

# include "Server.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <string>
// #include <initializer_list>

// enum class LogLevel {
//     INFO,
//     DEBUG,
//     WARNING,
//     ERROR
// };

// /**
//  * logMessage:
//  *  INFO, DEBUG, WARNING, ERROR
//  */
// void logMessage(LogLevel level, std::initializer_list<std::string> messages);

#ifndef DEBUG_MODE /* DEBUG_MODE */
# define DEBUG_MODE true /* Set to false to disable debug output */
#endif /* DEBUG_MODE */

inline void debugPrint(const std::string& msg) {
	if (DEBUG_MODE)
		std::cerr << "[DEBUG] " << msg << std::endl;
}

bool validatePort(const std::string& portStr, int& result);
bool validatePassword(const std::string& password);
