#pragma once

#include "Client.hpp"
#include "Utils.hpp"
#include "Server.hpp"
#include <string>

enum Commands {
    PASS,
    NICK,
    USER,
    JOIN,
    PART,
    PRIVMSG,
    INVALID,
};

class Client;

std::vector<std::string> splitReceivedBuffer(const std::string& str);
std::vector<std::string> splitCmd(std::string& cmd);