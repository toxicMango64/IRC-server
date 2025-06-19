#include "../inc/Client.hpp"

Client::Client(int fd): fd(fd) {
    isOperator = false;
    state = UNAUTHENTICATED; // Initialize state
}

Client::Client() {
	this->nickname = "";
	this->username = "";
	this->fd = -1;
	this->isOperator= false;
	this->state = UNAUTHENTICATED; // Initialize state
	this->buffer = "";
	this->ipadd = "";
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
		this->ipadd = src.ipadd;
		this->state = src.state; // Copy state
		this->isOperator = src.isOperator; // Ensure isOperator is copied
	}
	return *this;
}

int Client::GetFd( ) { return this->fd; }
ClientState Client::getState() const { return state; } // Implementation for new getter
bool Client::GetInviteChannel( std::string &ChName ) {
	for (size_t i = 0; i < this->ChannelsInvite.size(); i++) {
		if (this->ChannelsInvite[i] == ChName)
			return true;
	}
	return false;
}
std::string Client::GetNickName( ){ return this->nickname; }
std::string Client::GetUserName( ) { return this->username; }
std::string Client::getBuffer( ) { return buffer; }
std::string Client::getIpAdd( ) { return ipadd; }
std::string Client::getHostname( ) {
	std::string hostname = this->GetNickName() + "!" + this->GetUserName();
	return hostname;
}

void Client::SetFd( int fd ) { this->fd = fd; }
void Client::SetNickname( std::string& nickName ) { this->nickname = nickName; }
void Client::SetUsername(std::string& username){this->username = username; }
void Client::setBuffer(std::string recived){
    if (buffer.length() + recived.length() > 2048) { // Arbitrary limit, e.g., 4 times MAX_BUF
        // Disconnect client or handle error, for now just clear buffer to prevent crash
        buffer.clear();
        std::cerr << "Client buffer overflow prevented. Disconnecting client (FD: " << fd << ").\n";
        // In a real scenario, you would signal the server to disconnect this client.
        // For now, just clearing the buffer to prevent a crash.
        return;
    }
    buffer += recived;
}
void Client::setState(ClientState newState){state = newState; } // Implementation for new setter
void Client::setIpAdd(std::string ipadd){this->ipadd = ipadd; }

void Client::clearBuffer() { buffer.clear(); }
void Client::AddChannelInvite( std::string &chname ) {
	ChannelsInvite.push_back( chname );
}
void Client::RmChannelInvite( std::string &chname ) {
	for (size_t i = 0; i < this->ChannelsInvite.size(); i++) {
		if (this->ChannelsInvite[i] == chname)
			{this->ChannelsInvite.erase(this->ChannelsInvite.begin() + i); return;}
	}
}
