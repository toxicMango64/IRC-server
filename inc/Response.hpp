#pragma once

#include <string>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

#ifndef CRLF
#define CRLF "\r\n"
#endif

const std::string dalchawal = "irc.dal.chawal";

inline std::string RPL_CONNECTED(std::string nickname)
{
    return std::string(":") + std::string(dalchawal) + " 001 " + std::string(nickname) + " :Welcome to the " + std::string(dalchawal) + "!" + CRLF;
}

inline std::string RPL_YOURHOST(std::string nickname)
{
    return std::string(":") + std::string(dalchawal) + " 002 " + std::string(nickname) + " :Your host is " + std::string(dalchawal) + ", running version 1.0" + CRLF;
}

inline std::string RPL_CREATED(std::string nickname)
{
    std::time_t t = std::time(NULL);
    char time_buf[64];
    std::strftime(time_buf, sizeof(time_buf), "%a %b %d %Y at %H:%M:%S", std::localtime(&t));
    std::ostringstream oss;
    oss << ":" << dalchawal << " 003 " << nickname << " :This server was created " << time_buf << CRLF;
    return oss.str();
}


inline std::string RPL_MYINFO(std::string nickname)
{
    return std::string(":") + std::string(dalchawal) + " 004 " + std::string(nickname) + " " + std::string(dalchawal) + " 1.0  itkol klo" + CRLF;
}

inline std::string RPL_ISUPPORT(std::string nickname)
{
    return std::string(":") + std::string(dalchawal) + " 005 " + nickname +
           " CHANTYPES=# "
           "PREFIX=(o)@ "
           "CHANMODES=itkol "
           ":are supported by this server" +
           CRLF;
}

inline std::string RPL_CREATIONTIME(const std::string& nickname, const std::string& channelname, const std::string& creationtime) {
    return ":" + dalchawal + " 329 " + std::string(nickname) + " #" + std::string(channelname) + " " + std::string(creationtime) + CRLF;
}

inline std::string RPL_CHANNELMODES(const std::string& nickname, const std::string& channelname, const std::string& modes) {
    return ":" + dalchawal + " 324 " + std::string(nickname) + " #" + std::string(channelname) + " " + std::string(modes) + CRLF;
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
    return ":" + dalchawal + " 353 " + std::string(nickname) + " @ #" + std::string(channelname) + " :" + std::string(clientslist) + CRLF;
}

inline std::string RPL_ENDOFNAMES(const std::string& nickname, const std::string& channelname) {
    return ":" + dalchawal + " 366 " + std::string(nickname) + " #" + std::string(channelname) + " :End of /NAMES list" + CRLF;
}

inline std::string RPL_TOPICIS(const std::string& nickname, const std::string& channelname, const std::string& topic) {
    return ":" + dalchawal + " 332 " + std::string(nickname) + " #" + std::string(channelname) + " :" + std::string(topic) + CRLF;
}

inline std::string ERR_NEEDMODEPARM(const std::string& channelname, const std::string& mode) {
    return ": 696 #" + std::string(channelname) + " * You must specify a parameter for the mode " + std::string(mode) + "." + CRLF;
}

inline std::string ERR_INVALIDMODEPARM(const std::string& channelname, const std::string& mode) {
    return ": 696 #" + std::string(channelname) + " Invalid mode parameter: " + std::string(mode) + CRLF;
}

inline std::string ERR_KEYSET(const std::string& channelname) {
    return ":" + dalchawal + " 467 #" + std::string(channelname) + " Channel key already set." + CRLF;
}

inline std::string ERR_UNKNOWNMODE(const std::string& nickname, const std::string& channelname, const std::string& mode) {
    return ":" + dalchawal + " 472 " + std::string(nickname) + " #" + std::string(channelname) + " " + std::string(mode) + " :is not a recognised channel mode" + CRLF;
}

inline std::string ERR_NOTENOUGHPARAM(const std::string& nickname) {
    return ":" + dalchawal + " 461 " + std::string(nickname) + " :Not enough parameters." + CRLF;
}

inline std::string ERR_CHANNELNOTFOUND(const std::string& nickname, const std::string& channelname) {
    return ":" + dalchawal + " 403 " + std::string(nickname) + " " + std::string(channelname) + " :No such channel" + CRLF;
}

inline std::string ERR_NOTOPERATOR(const std::string& channelname) {
    return ":" + dalchawal + " 482 #" + std::string(channelname) + " :You're not a channel operator" + CRLF;
}

inline std::string ERR_NOSUCHNICK(const std::string& channelname, const std::string& name) {
    return ":" + dalchawal + " 401 #" + std::string(channelname) + " " + std::string(name) + " :No such nick/channel" + CRLF;
}

inline std::string ERR_INCORPASS(const std::string& nickname) {
    return ":" + dalchawal + " 464 " + std::string(nickname) + " :Password incorrect!" + CRLF;
}

inline std::string ERR_ALREADYREGISTERED(const std::string& nickname) {
    return ":" + dalchawal + " 462 " + std::string(nickname) + " :You may not reregister!" + CRLF;
}

inline std::string ERR_NONICKNAME(const std::string& nickname) {
    return ":" + dalchawal + " 431 " + std::string(nickname) + " :No nickname given" + CRLF;
}

inline std::string ERR_NICKINUSE(const std::string& nickname) {
    return ":" + dalchawal + " 433 " + std::string(nickname) + " :Nickname is already in use" + CRLF;
}

inline std::string ERR_ERRONEUSNICK(const std::string& nickname) {
    return ":" + dalchawal + " 432 " + std::string(nickname) + " :Erroneous nickname" + CRLF;
}

inline std::string ERR_NOTREGISTERED(const std::string& nickname) {
    return ":" + dalchawal + " 451 " + std::string(nickname) + " :You have not registered!" + CRLF;
}

inline std::string ERR_CMDNOTFOUND(const std::string& nickname, const std::string& command) {
    return ":" + dalchawal + " 421 " + std::string(nickname) + " " + std::string(command) + " :Unknown command" + CRLF;
}
