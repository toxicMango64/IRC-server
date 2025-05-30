// server.hpp
#pragma once

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

bool validatePort(const std::string& portStr, int& result);
bool validatePassword(const std::string& password);
