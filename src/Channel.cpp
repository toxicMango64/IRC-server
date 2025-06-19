#include "../inc/Channel.hpp"
#include <sys/socket.h>
#include <sstream>
#include <ctime>

Channel::Channel( ) {
	this->inviteOnly = 0;
	this->topic = 0;
	this->keyEnabled = 0;
	this->limit = 0;
	this->topicRestricted = false;
	this->name = "";
	this->topic_name = "";
	char charaters[5] = {'i', 't', 'k', 'o', 'l'};
	for(int i = 0; i < 5; i++)
		modes.push_back(std::make_pair(charaters[i],false));
	this->created_at = "";
}

Channel::~Channel( ){ }
Channel::Channel( Channel const &src ) { *this = src; }
Channel &Channel::operator=( Channel const &src ) {
	if (this != &src) {
		this->inviteOnly = src.inviteOnly;
		this->topic = src.topic;
		this->keyEnabled = src.keyEnabled;
		this->limit = src.limit;
		this->topicRestricted = src.topicRestricted;
		this->name = src.name;
		this->password = src.password;
		this->created_at = src.created_at;
		this->topic_name = src.topic_name;
		this->clients = src.clients;
		this->admins = src.admins;
		this->modes = src.modes;
	}
	return *this;
}

void Channel::SetInvitOnly(int inviteOnly){this->inviteOnly = inviteOnly;}
void Channel::SetTopic(int topic){this->topic = topic;}
void Channel::SetTime(std::string time){this->time_creation = time;}
void Channel::SetKey(int keyEnabled){this->keyEnabled = keyEnabled;}
void Channel::SetLimit(int limit){this->limit = limit;}
void Channel::SetTopicName(std::string topic_name){this->topic_name = topic_name;}
void Channel::SetPassword(std::string password){this->password = password;}
void Channel::SetName(std::string name){this->name = name;}
void Channel::set_topicRestriction(bool value){this->topicRestricted = value;}
void Channel::setModeAtindex(size_t index, bool mode){modes[index].second = mode;}
void Channel::set_createiontime() {
	std::time_t _time = std::time(NULL);
	std::ostringstream oss;
	oss << _time;
	this->created_at = std::string(oss.str());
}

int Channel::GetInvitOnly(){return this->inviteOnly;}
int Channel::GetTopic(){return this->topic;}
int Channel::GetKey(){return this->keyEnabled;}
int Channel::GetLimit(){return this->limit;}
int Channel::GetClientsNumber(){return this->clients.size() + this->admins.size();}

bool Channel::clientInChannel(std::string &nick){
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if (it->second.GetNickName() == nick)
			return true;
	}
	for (std::map<int, Client>::iterator it = admins.begin(); it != admins.end(); ++it) {
		if (it->second.GetNickName() == nick)
			return true;
	}
	return false;
}

Client *Channel::get_client(int fd){
	std::map<int, Client>::iterator it = clients.find(fd);
	if (it != clients.end())
		return &(it->second);
	return NULL;
}

Client *Channel::get_admin(int fd){
	std::map<int, Client>::iterator it = admins.find(fd);
	if (it != admins.end())
		return &(it->second);
	return NULL;
}

Client* Channel::GetClientInChannel(std::string name)
{
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if (it->second.GetNickName() == name)
			return &(it->second);
	}
	for (std::map<int, Client>::iterator it = admins.begin(); it != admins.end(); ++it) {
		if (it->second.GetNickName() == name)
			return &(it->second);
	}
	return NULL;
}

void Channel::add_client(Client newClient){clients[newClient.GetFd()] = newClient;}
void Channel::add_admin(Client newClient){admins[newClient.GetFd()] = newClient;}
void Channel::remove_client(int fd){clients.erase(fd);}
void Channel::remove_admin(int fd){admins.erase(fd);}

bool Channel::change_clientToAdmin(std::string& nick){
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if (it->second.GetNickName() == nick) {
			admins[it->first] = it->second;
			clients.erase(it->first);
			return true;
		}
	}
	return false;
}

bool Channel::change_adminToClient(std::string& nick){
	for (std::map<int, Client>::iterator it = admins.begin(); it != admins.end(); ++it) {
		if (it->second.GetNickName() == nick) {
			clients[it->first] = it->second;
			admins.erase(it->first);
			return true;
		}
	}
	return false;
}

std::string Channel::GetTopicName(){return this->topic_name;}
std::string Channel::GetPassword(){return this->password;}
std::string Channel::GetName(){return this->name;}
std::string Channel::GetTime(){return this->time_creation;}
std::string Channel::get_creationtime(){return created_at;}

std::string Channel::getModes(){
	std::string mode;
	for(size_t i = 0; i < modes.size(); i++) {
		if(modes[i].first != 'o' && modes[i].second)
			mode.push_back(modes[i].first);
	}
	if(!mode.empty())
		mode.insert(mode.begin(),'+');
	return mode;
}
std::string Channel::clientChannel_list(){
	std::string list;
	for (std::map<int, Client>::iterator it = admins.begin(); it != admins.end(); ++it) {
		list += "@" + it->second.GetNickName();
		list += " ";
	}
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
		list += it->second.GetNickName();
		list += " ";
	}
	if (!list.empty() && list[list.size()-1] == ' ')
		list.erase(list.size()-1, 1);
	return list;
}

void Channel::sendTo_all(std::string rpl1)
{
	for (std::map<int, Client>::iterator it = admins.begin(); it != admins.end(); ++it) {
		if (send(it->second.GetFd(), rpl1.c_str(), rpl1.size(),0) == -1) {
			std::cerr << "send() faild \n";
		}
	}
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if (send(it->second.GetFd(), rpl1.c_str(), rpl1.size(),0) == -1) {
			std::cerr << "send() faild \n";
		}
	}
}
void Channel::sendTo_all(std::string rpl1, int fd)
{
	for (std::map<int, Client>::iterator it = admins.begin(); it != admins.end(); ++it) {
		if (it->second.GetFd() != fd)
			if (send(it->second.GetFd(), rpl1.c_str(), rpl1.size(),0) == -1) {
				std::cerr << "send() faild \n";
			}
	}
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if (it->second.GetFd() != fd)
			if (send(it->second.GetFd(), rpl1.c_str(), rpl1.size(),0) == -1) {
				std::cerr << "send() faild \n";
			}
	}
}

bool Channel::getModeAtindex(size_t index) const {
	if (index < modes.size())
		return modes[index].second;
	return false;
}
bool Channel::Gettopic_restriction() const { return topicRestricted; }
