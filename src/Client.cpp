#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

char Client::buffer[MAX_BUF];

Client::Client(int fd): fd(fd) {
	this->nickname = "";
	this->username = "";
	this->isOperator= false;
	this->registered = false;
	std::memset(this->buffer, 0, MAX_BUF);
	this->ipadd = "";
	this->logedin = false;
}
Client::Client(void) {
	*this = Client(-1);
}
Client::Client(const std::string& nickname, const std::string& username, int fd) {
	*this = Client(fd);
	this->username = username;
	this->nickname = nickname;
}
Client::~Client(void) { }
Client::Client(Client const &src) { *this = src; }

Client &Client::operator=(Client const &src){
	if (this != &src){
		this->nickname = src.nickname;
		this->username = src.username;
		this->fd = src.fd;
		this->ChannelsInvite = src.ChannelsInvite;
		std::copy(src.buffer, src.buffer + MAX_BUF, this->buffer);
		this->registered = src.registered;
		this->ipadd = src.ipadd;
		this->logedin = src.logedin;
		this->_outgoingBuffer = src._outgoingBuffer; // Copy outgoing buffer
	}
	return *this;
}

int Client::GetFd( ) { return this->fd; }
bool Client::getRegistered( ) { return registered; }
bool Client::GetInviteChannel( std::string &ChName ) {
	for (size_t i = 0; i < this->ChannelsInvite.size(); i++) {
		if (this->ChannelsInvite[i] == ChName)
			return true;
	}
	return false;
}
std::string Client::GetNickName( ){ return this->nickname; }
bool Client::GetLogedIn( ) { return this->logedin; }
std::string Client::GetUserName( ) { return this->username; }
std::string Client::getBuffer( ) { return buffer; }
std::string Client::getIpAdd( ) { return ipadd; }
std::string Client::getHostname( ) {
	std::string hostname = this->GetNickName() + "!~" + this->GetUserName() + "@" + "irc.dal.chawal";
	return hostname;
}

std::string& Client::getOutgoingBuffer() {
    return _outgoingBuffer;
}

void Client::SetFd( int fd ) { this->fd = fd; }
void Client::SetNickname( std::string& nickName ) { this->nickname = nickName; }
void Client::setLogedin(bool value){this->logedin = value; }
void Client::SetUsername(std::string& username){this->username = username; }

void Client::setBuffer(std::string recived) {
	// recived.erase(std::remove(recived.begin(), recived.end(), '\x04'), recived.end());
    // if (buffer.length() + recived.length() > Server::MAX_BUF * 4) { 
	if (strlen(buffer) + recived.length() > MAX_BUF * 4) { 
		buffer[0] = '\0';
		std::cerr << "Client buffer overflow prevented. Disconnecting client (FD: " << fd << ").\n";
		return ;
	}
	strncat(buffer, recived.c_str(), sizeof(buffer) - strlen(buffer) - 1);
}

void Client::setRegistered(bool value){registered = value; }
void Client::setIpAdd(std::string ipadd){this->ipadd = ipadd; }

void Client::appendOutgoing(const std::string& data) {
    _outgoingBuffer.append(data);
}

void Client::clearOutgoingBuffer(size_t bytesSent) {
    if (bytesSent >= _outgoingBuffer.size()) {
        _outgoingBuffer.clear();
    } else {
        _outgoingBuffer.erase(0, bytesSent);
    }
}

void Client::clearBuffer() { buffer[0] = '\0'; }
void Client::AddChannelInvite( std::string &chname ) {
	ChannelsInvite.push_back( chname );
}
void Client::RmChannelInvite( std::string &chname ) {
	for (size_t i = 0; i < this->ChannelsInvite.size(); i++) {
		if (this->ChannelsInvite[i] == chname)
			{this->ChannelsInvite.erase(this->ChannelsInvite.begin() + i); return;}
	}
}
