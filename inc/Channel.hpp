// Channel.hpp
#pragma once

#include <string>
#include <set>
#include <map>

struct ChannelMode {
    bool        inviteOnly = false;
    bool        topicRestricted = false;
    bool        keyEnabled = false;
    std::string key;
    bool        clientLimitEnabled = false;
    int         clientLimit = 0;
};

class Channel {
    public:
        std::string     name;
        std::set<int>   members;
        std::set<int>   operators;
        ChannelMode     mode;
        std::string     topic;

        Channel( const std::string& name, int creatorFd );
};
