// server.hpp
#pragma once

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <string>

bool validatePort(const std::string& portStr, int& result);
bool validatePassword(const std::string& password);
