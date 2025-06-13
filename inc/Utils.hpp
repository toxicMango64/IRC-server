// Utils.hpp
#pragma once

#ifndef DEBUG_MODE /* DEBUG_MODE */
# define DEBUG_MODE true /* Set to false to disable debug output */
#endif /* DEBUG_MODE */

// # include "Server.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <cstdarg>
#include <cerrno>
#include <cstring>

bool validatePort( const std::string& portStr, int& result );
bool validatePassword( const std::string& password );

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

inline void debugPrint(const std::string& msg) {
	if (DEBUG_MODE)
	std::cerr << "[DEBUG] " << msg << "\n";
}

// Error: file name: {README.}: No such file or directory
inline void log_error(const char* format, ...) {
	va_list args;
	va_start(args, format);

	std::stringstream error_message;

	for (const char* p = format; *p != '\0'; ++p) {
		if (*p == '%' && *(p + 1) != '\0') {
			++p;
			if (*p == 's') {
				const char* str = va_arg(args, const char*);
				error_message << str;
			} else if (*p == 'd' || *p == 'i') {
				int num = va_arg(args, int);
				error_message << num;
			} else if (*p == 'x') {
				int num = va_arg(args, int);
				error_message << std::hex << num << std::dec;
			} else {
				error_message << "%" << *p;
			}
		} else {
			error_message.put(*p);
		}
	}

	va_end(args);

	// Append errno message if errno is set
	if (errno != 0) {
		error_message << ": { " << std::strerror(errno) << " }";
	}

	std::cerr << "Error: " << error_message.str() << "\n";
}

// int main(int ac, char **av) {

// 	(void) ac;
// 	// Attempt to open a non-existent file to set errno
// 	FILE* file = fopen(av[1], "r");
// 	if (file == nullptr) {
// 		log_error("file name: { %s }", av[1]);
// 	} else {
// 		fclose(file); // Close the file if it was opened successfully
// 	}
// 	return 0;
// }
