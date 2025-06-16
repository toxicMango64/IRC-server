#include "../inc/Client.hpp"

Client::Client(int fd): fd(fd) {
    isOperator = false;
    state = UNAUTHENTICATED;
}

Client::Client() {
	this->nickname = "";
	this->username = "";
	this->fd = -1;
	this->isOperator= false;
	this->registered = false;
	this->buffer = "";
	this->ipadd = "";
	this->logedin = false;
}

Client::Client(std::string nickname, std::string username, int fd) :fd(fd), nickname(nickname), username(username) { }
Client::~Client( ) { }
Client::Client( Client const &src ) { *this = src; }

Client &Client::operator=(Client const &src){
	if (this != &src){
		this->nickname = src.nickname;
		this->username = src.username;
		this->fd = src.fd;
		this->ChannelsInvite = src.ChannelsInvite;
		this->buffer = src.buffer;
		this->registered = src.registered;
		this->ipadd = src.ipadd;
		this->logedin = src.logedin;
	}
	return *this;
}
