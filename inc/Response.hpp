#pragma once

#include <string>
#include <iostream>
#include <string_view>

#ifndef CRLF
# define CRLF "\r\n";
#endif 
// extern const std::string CRLF = "\r\n";

inline std::string RPL_CONNECTED(const std::string& nickname) {
    return ": 001 " + std::string(nickname) + " :Welcome to the IRC server!" + CRLF;
}

inline std::string RPL_UMODEIS(const std::string& hostname, const std::string& channelname,
                               const std::string& mode, const std::string& user) {
    return ":" + std::string(hostname) + " MODE " + std::string(channelname) + " " +
           std::string(mode) + " " + std::string(user) + CRLF;
}

inline std::string RPL_CHANNELMODE_GENERIC(const std::string& nickname, const std::string& channelname, const std::string& param, const std::string& rpl_code) {
    return ":" + rpl_code + " " + std::string(nickname) + " #" + std::string(channelname) + " " + std::string(param) + CRLF;
}

inline std::string RPL_CREATIONTIME(const std::string& nickname, const std::string& channelname, const std::string& creationtime) {
    return RPL_CHANNELMODE_GENERIC(nickname, channelname, creationtime, "329");
}

inline std::string RPL_CHANNELMODES(const std::string& nickname, const std::string& channelname, const std::string& modes) {
    return RPL_CHANNELMODE_GENERIC(nickname, channelname, modes, "324");
}

inline std::string RPL_CHANGEMODE(const std::string& hostname, const std::string& channelname,
                                  const std::string& mode, const std::string& arguments) {
    return ":" + std::string(hostname) + " MODE #" + std::string(channelname) + " " +
           std::string(mode) + " " + std::string(arguments) + CRLF;
}

inline std::string RPL_NICKCHANGE(const std::string& oldnickname, const std::string& newnickname) {
    return ":" + std::string(oldnickname) + " NICK " + std::string(newnickname) + CRLF;
}

inline std::string RPL_JOINMSG(const std::string& hostname, const std::string& ipaddress, const std::string& channelname) {
    return ":" + std::string(hostname) + "@" + std::string(ipaddress) + " JOIN #" + std::string(channelname) + CRLF;
}

inline std::string RPL_NAMREPLY(const std::string& nickname, const std::string& channelname, const std::string& clientslist) {
    return ": 353 " + std::string(nickname) + " @ #" + std::string(channelname) + " :" + std::string(clientslist) + CRLF;
}

inline std::string RPL_ENDOFNAMES(const std::string& nickname, const std::string& channelname) {
    return ": 366 " + std::string(nickname) + " #" + std::string(channelname) + " :End of /NAMES list" + CRLF;
}

inline std::string RPL_TOPICIS(const std::string& nickname, const std::string& channelname, const std::string& topic) {
    return ": 332 " + std::string(nickname) + " #" + std::string(channelname) + " :" + std::string(topic) + CRLF;
}

// New RPL messages for registration
inline std::string RPL_WELCOME(const std::string& nickname, const std::string& username) {
    return ":localhost 001 " + nickname + " :Welcome to the ft_irc Network, " + nickname + "!" + username + "@localhost" + CRLF;
}

inline std::string RPL_YOURHOST(const std::string& nickname) {
    return ":localhost 002 " + nickname + " :Your host is localhost, running version 1.0" + CRLF;
}

inline std::string RPL_CREATED(const std::string& nickname) {
    return ":localhost 003 " + nickname + " :This server was created " + __DATE__ + " at " + __TIME__ + CRLF;
}

inline std::string RPL_MYINFO(const std::string& nickname) {
    return ":localhost 004 " + nickname + " localhost 1.0 aoOirw" + CRLF;
}


// Error messages

inline std::string ERR_NEEDMODEPARM(const std::string& channelname, const std::string& mode) {
    return ": 696 #" + std::string(channelname) + " * You must specify a parameter for the mode " + std::string(mode) + "." + CRLF;
}

inline std::string ERR_INVALIDMODEPARM(const std::string& channelname, const std::string& mode) {
    return ": 696 #" + std::string(channelname) + " Invalid mode parameter: " + std::string(mode) + CRLF;
}

inline std::string ERR_KEYSET(const std::string& channelname) {
    return ": 467 #" + std::string(channelname) + " Channel key already set." + CRLF;
}

inline std::string ERR_UNKNOWNMODE(const std::string& nickname, const std::string& channelname, const std::string& mode) {
    return ": 472 " + std::string(nickname) + " #" + std::string(channelname) + " " + std::string(mode) + " :is not a recognised channel mode" + CRLF;
}

// Corrected error messages
inline std::string ERR_NEEDMOREPARAMS(const std::string& nickname, const std::string& command) {
    return ":localhost 461 " + nickname + " " + command + " :Not enough parameters" + CRLF;
}

inline std::string ERR_CHANNELNOTFOUND(const std::string& nickname, const std::string& channelname) {
    return ": 403 " + std::string(nickname) + " " + std::string(channelname) + " :No such channel" + CRLF;
}

inline std::string ERR_NOTOPERATOR(const std::string& channelname) {
    return ": 482 #" + std::string(channelname) + " :You're not a channel operator" + CRLF;
}

inline std::string ERR_NOSUCHNICK(const std::string& channelname, const std::string& name) {
    return ": 401 #" + std::string(channelname) + " " + std::string(name) + " :No such nick/channel" + CRLF;
}

inline std::string ERR_PASSWDMISMATCH(const std::string& nickname) {
    return ":localhost 464 " + nickname + " :Password incorrect" + CRLF;
}

inline std::string ERR_ALREADYREGISTERED(const std::string& nickname) {
    return ":localhost 462 " + nickname + " :You may not reregister" + CRLF;
}

inline std::string ERR_NONICKNAMEGIVEN(const std::string& nickname) {
    return ":localhost 431 " + nickname + " :No nickname given" + CRLF;
}

inline std::string ERR_NICKNAMEINUSE(const std::string& nickname, const std::string& newnickname) {
    return ":localhost 433 " + nickname + " " + newnickname + " :Nickname is already in use" + CRLF;
}

inline std::string ERR_ERRONEUSNICKNAME(const std::string& nickname, const std::string& newnickname) {
    return ":localhost 432 " + nickname + " " + newnickname + " :Erroneous nickname" + CRLF;
}

inline std::string ERR_NOTREGISTERED(const std::string& nickname) {
    return ":localhost 451 " + nickname + " :You have not registered" + CRLF;
}

inline std::string ERR_CMDNOTFOUND(const std::string& nickname, const std::string& command) {
    return ":localhost 421 " + nickname + " " + command + " :Unknown command" + CRLF;
}
