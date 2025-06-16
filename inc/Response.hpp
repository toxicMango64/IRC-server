#pragma once

#include <string>
#include <string_view>

const char* CRLF = "\r\n";

inline std::string RPL_CONNECTED(std::string_view nickname) {
    return ": 001 " + std::string(nickname) + " :Welcome to the IRC server!" + CRLF;
}

inline std::string RPL_UMODEIS(std::string_view hostname, std::string_view channelname,
                               std::string_view mode, std::string_view user) {
    return ":" + std::string(hostname) + " MODE " + std::string(channelname) + " " +
           std::string(mode) + " " + std::string(user) + CRLF;
}

inline std::string RPL_CREATIONTIME(std::string_view nickname, std::string_view channelname, std::string_view creationtime) {
    return ": 329 " + std::string(nickname) + " #" + std::string(channelname) + " " + std::string(creationtime) + CRLF;
}

inline std::string RPL_CHANNELMODES(std::string_view nickname, std::string_view channelname, std::string_view modes) {
    return ": 324 " + std::string(nickname) + " #" + std::string(channelname) + " " + std::string(modes) + CRLF;
}

inline std::string RPL_CHANGEMODE(std::string_view hostname, std::string_view channelname,
                                  std::string_view mode, std::string_view arguments) {
    return ":" + std::string(hostname) + " MODE #" + std::string(channelname) + " " +
           std::string(mode) + " " + std::string(arguments) + CRLF;
}

inline std::string RPL_NICKCHANGE(std::string_view oldnickname, std::string_view newnickname) {
    return ":" + std::string(oldnickname) + " NICK " + std::string(newnickname) + CRLF;
}

inline std::string RPL_JOINMSG(std::string_view hostname, std::string_view ipaddress, std::string_view channelname) {
    return ":" + std::string(hostname) + "@" + std::string(ipaddress) + " JOIN #" + std::string(channelname) + CRLF;
}

inline std::string RPL_NAMREPLY(std::string_view nickname, std::string_view channelname, std::string_view clientslist) {
    return ": 353 " + std::string(nickname) + " @ #" + std::string(channelname) + " :" + std::string(clientslist) + CRLF;
}

inline std::string RPL_ENDOFNAMES(std::string_view nickname, std::string_view channelname) {
    return ": 366 " + std::string(nickname) + " #" + std::string(channelname) + " :End of /NAMES list" + CRLF;
}

inline std::string RPL_TOPICIS(std::string_view nickname, std::string_view channelname, std::string_view topic) {
    return ": 332 " + std::string(nickname) + " #" + std::string(channelname) + " :" + std::string(topic) + CRLF;
}

// Error messages

inline std::string ERR_NEEDMODEPARM(std::string_view channelname, std::string_view mode) {
    return ": 696 #" + std::string(channelname) + " * You must specify a parameter for the mode " + std::string(mode) + "." + CRLF;
}

inline std::string ERR_INVALIDMODEPARM(std::string_view channelname, std::string_view mode) {
    return ": 696 #" + std::string(channelname) + " Invalid mode parameter: " + std::string(mode) + CRLF;
}

inline std::string ERR_KEYSET(std::string_view channelname) {
    return ": 467 #" + std::string(channelname) + " Channel key already set." + CRLF;
}

inline std::string ERR_UNKNOWNMODE(std::string_view nickname, std::string_view channelname, std::string_view mode) {
    return ": 472 " + std::string(nickname) + " #" + std::string(channelname) + " " + std::string(mode) + " :is not a recognised channel mode" + CRLF;
}

inline std::string ERR_NOTENOUGHPARAM(std::string_view nickname) {
    return ": 461 " + std::string(nickname) + " :Not enough parameters." + CRLF;
}

inline std::string ERR_CHANNELNOTFOUND(std::string_view nickname, std::string_view channelname) {
    return ": 403 " + std::string(nickname) + " " + std::string(channelname) + " :No such channel" + CRLF;
}

inline std::string ERR_NOTOPERATOR(std::string_view channelname) {
    return ": 482 #" + std::string(channelname) + " :You're not a channel operator" + CRLF;
}

inline std::string ERR_NOSUCHNICK(std::string_view channelname, std::string_view name) {
    return ": 401 #" + std::string(channelname) + " " + std::string(name) + " :No such nick/channel" + CRLF;
}

inline std::string ERR_INCORPASS(std::string_view nickname) {
    return ": 464 " + std::string(nickname) + " :Password incorrect!" + CRLF;
}

inline std::string ERR_ALREADYREGISTERED(std::string_view nickname) {
    return ": 462 " + std::string(nickname) + " :You may not reregister!" + CRLF;
}

inline std::string ERR_NONICKNAME(std::string_view nickname) {
    return ": 431 " + std::string(nickname) + " :No nickname given" + CRLF;
}

inline std::string ERR_NICKINUSE(std::string_view nickname) {
    return ": 433 " + std::string(nickname) + " :Nickname is already in use" + CRLF;
}

inline std::string ERR_ERRONEUSNICK(std::string_view nickname) {
    return ": 432 " + std::string(nickname) + " :Erroneous nickname" + CRLF;
}

inline std::string ERR_NOTREGISTERED(std::string_view nickname) {
    return ": 451 " + std::string(nickname) + " :You have not registered!" + CRLF;
}

inline std::string ERR_CMDNOTFOUND(std::string_view nickname, std::string_view command) {
    return ": 421 " + std::string(nickname) + " " + std::string(command) + " :Unknown command" + CRLF;
}
