#pragma once

#include <string>
#include <vector>
#include "Client.hpp"

static const int MAX_CLIENT_LIMIT = 5;

class Client;
class Channel {
private:
	int		inviteOnly;
	int		topic;
	int		keyEnabled;
	int		limit;
	bool	topicRestricted;

	std::string	name;
	std::string	time_creation;
	std::string	password;
	std::string	created_at;
	std::string	topic_name;
	std::vector<Client>	clients;
	std::vector<Client>	admins;
	std::vector<std::pair<char, bool> >	modes;
	bool clientListBeingModified;

public:
	Channel( );
	~Channel( );
	Channel( Channel const &src );
	Channel &operator=( Channel const &src );

	void SetInvitOnly( int inviteOnly );
	void SetTopic( int topic );
	void SetKey( int keyEnabled );
	void SetLimit( int limit );
	void SetTopicName( std::string topic_name );
	void SetPassword( std::string password );
	void SetName( std::string name );
	void SetTime( std::string time );
	void set_topicRestriction( bool value );
	void setModeAtindex( size_t index, bool mode );
	void set_createiontime(  );

	int GetInvitOnly(  );
	int GetTopic(  );
	int GetKey(  );
	int GetLimit(  );
	int GetClientsNumber(  );
	bool Gettopic_restriction( ) const;
	bool getModeAtindex( size_t index );
	bool clientInChannel( std::string &nick );
	std::string GetTopicName(  );
	std::string GetPassword(  );
	std::string GetName(  );
	std::string GetTime(  );
	std::string get_creationtime(  );
	std::string getModes(  );
	std::string clientChannel_list(  );
	Client *get_client( int fd );
	Client *get_admin( int fd );
	Client *GetClientInChannel( std::string name );

	void add_client( Client newClient );
	void add_admin( Client newClient );
	void remove_client( int fd );
	void remove_admin( int fd );
	bool change_clientToAdmin( std::string& nick );
	bool change_adminToClient( std::string& nick );

	void sendToAll( std::string msg );
	void sendToAllExcept(const std::string& message, int excludeFd);

};
