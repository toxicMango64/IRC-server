#pragma once

#include <string>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

#ifndef CRLF
# define CRLF "\r\n"
#endif

// Server name constant for consistency
const std::string dalchawal = "irc.dal.chwal";

inline std::string RPL_CONNECTED(std::string nickname) {
    return std::string(":") + std::string(dalchawal) + " 001 " + std::string(nickname) + " :Welcome to the " + std::string(dalchawal) + "!" + CRLF;
}

inline std::string RPL_YOURHOST(std::string nickname) {
    return std::string(":") + std::string(dalchawal) + " 002 " + std::string(nickname) + " :Your host is " + std::string(dalchawal) + ", running version 1.0" + CRLF;
}

inline std::string RPL_CREATED(std::string nickname) {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%a %b %d %Y at %H:%M:%S %z");
    return std::string(":") + std::string(dalchawal) + " 003 " + std::string(nickname) + " :This server was created " + oss.str() + CRLF;
}

inline std::string RPL_MYINFO(std::string nickname) {
    return std::string(":") + std::string(dalchawal) + " 004 " + std::string(nickname) + " " + std::string(dalchawal) + " 1.0 o o o o" + CRLF;
}

inline std::string RPL_ISUPPORT(std::string nickname) {
    return std::string(":") + std::string(dalchawal) + " 005 " + std::string(nickname) + " CHANTYPES=# PREFIX=(ohv)@%+ :are supported by this server" + CRLF;
}

inline std::string RPL_UMODEIS(std::string hostname, std::string channelname, std::string mode, std::string user) {
    return ":" + std::string(hostname) + " MODE " + std::string(channelname) + " " + std::string(mode) + " " + std::string(user) + CRLF;
}

inline std::string RPL_CREATIONTIME(std::string nickname, std::string channelname, std::string creationtime) {
    return ": 329 " + std::string(nickname) + " #" + std::string(channelname) + " " + std::string(creationtime) + CRLF;
}

inline std::string RPL_CHANNELMODES(std::string nickname, std::string channelname, std::string modes) {
    return ": 324 " + std::string(nickname) + " #" + std::string(channelname) + " " + std::string(modes) + CRLF;
}

inline std::string RPL_CHANGEMODE(std::string hostname, std::string channelname, std::string mode, std::string arguments) {
    return ":" + std::string(hostname) + " MODE #" + std::string(channelname) + " " + std::string(mode) + " " + std::string(arguments) + CRLF;
}

inline std::string RPL_NICKCHANGE(std::string oldnickname, std::string newnickname) {
    return ":" + std::string(oldnickname) + " NICK " + std::string(newnickname) + CRLF;
}

inline std::string RPL_JOINMSG(std::string hostname, std::string ipaddress, std::string channelname) {
    return ":" + std::string(hostname) + "@" + std::string(ipaddress) + " JOIN #" + std::string(channelname) + CRLF;
}

inline std::string RPL_NAMREPLY(std::string nickname, std::string channelname, std::string clientslist) {
    return ": 353 " + std::string(nickname) + " @ #" + std::string(channelname) + " :" + std::string(clientslist) + CRLF;
}

inline std::string RPL_ENDOFNAMES(std::string nickname, std::string channelname) {
    return ": 366 " + std::string(nickname) + " #" + std::string(channelname) + " :End of /NAMES list" + CRLF;
}

inline std::string RPL_TOPICIS(std::string nickname, std::string channelname, std::string topic) {
    return ": 332 " + std::string(nickname) + " #" + std::string(channelname) + " :" + std::string(topic) + CRLF;
}

// Error messages

inline std::string ERR_NEEDMODEPARM(std::string channelname, std::string mode) {
    return ": 696 #" + std::string(channelname) + " * You must specify a parameter for the mode " + std::string(mode) + "." + CRLF;
}

inline std::string ERR_INVALIDMODEPARM(std::string channelname, std::string mode) {
    return ": 696 #" + std::string(channelname) + " Invalid mode parameter: " + std::string(mode) + CRLF;
}

inline std::string ERR_KEYSET(std::string channelname) {
    return ": 467 #" + std::string(channelname) + " Channel key already set." + CRLF;
}

inline std::string ERR_UNKNOWNMODE(std::string nickname, std::string channelname, std::string mode) {
    return ": 472 " + std::string(nickname) + " #" + std::string(channelname) + " " + std::string(mode) + " :is not a recognised channel mode" + CRLF;
}

inline std::string ERR_NOTENOUGHPARAM(std::string nickname) {
    return ": 461 " + std::string(nickname) + " :Not enough parameters." + CRLF;
}

inline std::string ERR_CHANNELNOTFOUND(std::string nickname, std::string channelname) {
    return ": 403 " + std::string(nickname) + " " + std::string(channelname) + " :No such channel" + CRLF;
}

inline std::string ERR_NOTOPERATOR(std::string channelname) {
    return ": 482 #" + std::string(channelname) + " :You're not a channel operator" + CRLF;
}

inline std::string ERR_NOSUCHNICK(std::string channelname, std::string name) {
    return ": 401 #" + std::string(channelname) + " " + std::string(name) + " :No such nick/channel" + CRLF;
}

inline std::string ERR_INCORPASS(std::string nickname) {
    return ": 464 " + std::string(nickname) + " :Password incorrect!" + CRLF;
}

inline std::string ERR_ALREADYREGISTERED(std::string nickname) {
    return ": 462 " + std::string(nickname) + " :You may not reregister!" + CRLF;
}

inline std::string ERR_NONICKNAME(std::string nickname) {
    return ": 431 " + std::string(nickname) + " :No nickname given" + CRLF;
}

inline std::string ERR_NICKINUSE(std::string nickname) {
    return ": 433 " + std::string(nickname) + " :Nickname is already in use" + CRLF;
}

inline std::string ERR_ERRONEUSNICK(std::string nickname) {
    return ": 432 " + std::string(nickname) + " :Erroneous nickname" + CRLF;
}

inline std::string ERR_NOTREGISTERED(std::string nickname) {
    return ": 451 " + std::string(nickname) + " :You have not registered!" + CRLF;
}

inline std::string ERR_CMDNOTFOUND(std::string nickname, std::string command) {
    return ": 421 " + std::string(nickname) + " " + std::string(command) + " :Unknown command" + CRLF;
}


// OURRRRSSS:
// #pragma once

// #include <string>
// #include <iostream>
// #include <string_view>
// #include <ctime>
// #include <iomanip>
// #include <sstream>

// #ifndef CRLF
// # define CRLF "\r\n";
// #endif
// // extern const std::string CRLF = "\r\n";

// inline std::string RPL_CONNECTED(const std::string& nickname) {
//     return ":irc.dal.chwal 001 " std::string(nickname) + " :Welcome to the irc.dal.chwal!" + std::string(nickname) + CRLF;
// }

// // 002 RPL_YOURHOST
// inline std::string RPL_YOURHOST(const std::string& nickname) {
//     return ":irc.dal.chwal 002 " + std::string(nickname) + " :Your host is irc.dal.chwal, running version 1.0" + CRLF;
// }

// // 003 RPL_CREATED
// inline std::string RPL_CREATED(const std::string& nickname) {
//     std::time_t t = std::time(nullptr);
//     std::tm tm = *std::localtime(&t);

//     std::ostringstream oss;
//     oss << std::put_time(&tm, "%a %b %d %Y at %H:%M:%S %z");
//     std::string time_str = oss.str();

//     return ":irc.dal.chwal 003 " + nickname + " :This server was created " + time_str + CRLF;
// }
// // inline std::string RPL_CREATED(const std::string& nickname) {
// //     return std::string(nickname) + " :This server was created Fri Jun 20 2025 at 17:01:00 +04" + CRLF; ///////////////////////// needs change
// // }

// // 004 RPL_MYINFO
// inline std::string RPL_MYINFO(const std::string& nickname) {
//     // <servername> <version> <available user modes> <available channel modes>
//     return ":irc.dal.chwal 004 " + nickname + " irc.dal.chwal 1.0 o o o o" + CRLF;
// }

// // 005 RPL_ISUPPORT
// inline std::string RPL_ISUPPORT(const std::string& nickname) {
//     // Add actual ISUPPORT tokens as appropriate for your server
//     return ":irc.dal.chwal 005 " + nickname + " CHANTYPES=#& PREFIX=(ohv)@%+ :are supported by this server" + CRLF;
// }

// inline std::string RPL_UMODEIS(const std::string& hostname, const std::string& channelname,
//                                const std::string& mode, const std::string& user) {
//     return ":" + std::string(hostname) + " MODE " + std::string(channelname) + " " +
//            std::string(mode) + " " + std::string(user) + CRLF;
// }

// inline std::string RPL_CREATIONTIME(const std::string& nickname, const std::string& channelname, const std::string& creationtime) {
//     return ": 329 " + std::string(nickname) + " #" + std::string(channelname) + " " + std::string(creationtime) + CRLF;
// }

// inline std::string RPL_CHANNELMODES(const std::string& nickname, const std::string& channelname, const std::string& modes) {
//     return ": 324 " + std::string(nickname) + " #" + std::string(channelname) + " " + std::string(modes) + CRLF;
// }

// inline std::string RPL_CHANGEMODE(const std::string& hostname, const std::string& channelname,
//                                   const std::string& mode, const std::string& arguments) {
//     return ":" + std::string(hostname) + " MODE #" + std::string(channelname) + " " +
//            std::string(mode) + " " + std::string(arguments) + CRLF;
// }

// inline std::string RPL_NICKCHANGE(const std::string& oldnickname, const std::string& newnickname) {
//     return ":" + std::string(oldnickname) + " NICK " + std::string(newnickname) + CRLF;
// }

// inline std::string RPL_JOINMSG(const std::string& hostname, const std::string& ipaddress, const std::string& channelname) {
//     return ":" + std::string(hostname) + "@" + std::string(ipaddress) + " JOIN #" + std::string(channelname) + CRLF;
// }

// inline std::string RPL_NAMREPLY(const std::string& nickname, const std::string& channelname, const std::string& clientslist) {
//     return ": 353 " + std::string(nickname) + " @ #" + std::string(channelname) + " :" + std::string(clientslist) + CRLF;
// }

// inline std::string RPL_ENDOFNAMES(const std::string& nickname, const std::string& channelname) {
//     return ": 366 " + std::string(nickname) + " #" + std::string(channelname) + " :End of /NAMES list" + CRLF;
// }

// inline std::string RPL_TOPICIS(const std::string& nickname, const std::string& channelname, const std::string& topic) {
//     return ": 332 " + std::string(nickname) + " #" + std::string(channelname) + " :" + std::string(topic) + CRLF;
// }

// // Error messages

// inline std::string ERR_NEEDMODEPARM(const std::string& channelname, const std::string& mode) {
//     return ": 696 #" + std::string(channelname) + " * You must specify a parameter for the mode " + std::string(mode) + "." + CRLF;
// }

// inline std::string ERR_INVALIDMODEPARM(const std::string& channelname, const std::string& mode) {
//     return ": 696 #" + std::string(channelname) + " Invalid mode parameter: " + std::string(mode) + CRLF;
// }

// inline std::string ERR_KEYSET(const std::string& channelname) {
//     return ": 467 #" + std::string(channelname) + " Channel key already set." + CRLF;
// }

// inline std::string ERR_UNKNOWNMODE(const std::string& nickname, const std::string& channelname, const std::string& mode) {
//     return ": 472 " + std::string(nickname) + " #" + std::string(channelname) + " " + std::string(mode) + " :is not a recognised channel mode" + CRLF;
// }

// inline std::string ERR_NOTENOUGHPARAM(const std::string& nickname) {
//     return ": 461 " + std::string(nickname) + " :Not enough parameters." + CRLF;
// }

// inline std::string ERR_CHANNELNOTFOUND(const std::string& nickname, const std::string& channelname) {
//     return ": 403 " + std::string(nickname) + " " + std::string(channelname) + " :No such channel" + CRLF;
// }

// inline std::string ERR_NOTOPERATOR(const std::string& channelname) {
//     return ": 482 #" + std::string(channelname) + " :You're not a channel operator" + CRLF;
// }

// inline std::string ERR_NOSUCHNICK(const std::string& channelname, const std::string& name) {
//     return ": 401 #" + std::string(channelname) + " " + std::string(name) + " :No such nick/channel" + CRLF;
// }

// inline std::string ERR_INCORPASS(const std::string& nickname) {
//     return ": 464 " + std::string(nickname) + " :Password incorrect!" + CRLF;
// }

// inline std::string ERR_ALREADYREGISTERED(const std::string& nickname) {
//     return ": 462 " + std::string(nickname) + " :You may not reregister!" + CRLF;
// }

// inline std::string ERR_NONICKNAME(const std::string& nickname) {
//     return ": 431 " + std::string(nickname) + " :No nickname given" + CRLF;
// }

// inline std::string ERR_NICKINUSE(const std::string& nickname) {
//     return ": 433 " + std::string(nickname) + " :Nickname is already in use" + CRLF;
// }

// inline std::string ERR_ERRONEUSNICK(const std::string& nickname) {
//     return ": 432 " + std::string(nickname) + " :Erroneous nickname" + CRLF;
// }

// inline std::string ERR_NOTREGISTERED(const std::string& nickname) {
//     return ": 451 " + std::string(nickname) + " :You have not registered!" + CRLF;
// }

// inline std::string ERR_CMDNOTFOUND(const std::string& nickname, const std::string& command) {
//     return ": 421 " + std::string(nickname) + " " + std::string(command) + " :Unknown command" + CRLF;
// }
