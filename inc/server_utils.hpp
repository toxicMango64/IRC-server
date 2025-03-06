// server.h
#ifndef SERVER_UTILS_H
# define SERVER_UTILS_H

#pragma once
#include <string>

bool validatePort(const std::string& portStr, int& result);
bool validatePassword(const std::string& password);

#endif