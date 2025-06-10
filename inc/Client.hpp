// Client.hpp
#pragma once

#include <string>
#include <set>

#include "Utils.hpp"

enum ClientState {
    UNAUTHENTICATED,
    AUTHENTICATED,
    REGISTERED,
};

class Client {
    public:
        int                     fd;
        std::string             nickname;
        std::string             realname;
        std::string             username;
        ClientState             state;
        std::set<std::string>   channels;
        bool                    isOperator;

        Client( int fd );
};
