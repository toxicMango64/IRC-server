// Channel.hpp
#pragma once

#include <string>
#include <vector>
#include <map>
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
	// int 	clientLimit;
	// bool 	clientLimitEnabled;
	std::string	name;
	std::string	time_creation;
	std::string	password;
	std::string	created_at;
	std::string	topic_name;
	std::map<int, Client> clients;
	std::map<int, Client> admins;
	std::vector<std::pair<char, bool> > modes;

public:
	// Constructor to initialize members
	// Channel(  )
	// 	: inviteOnly( false ),
	// 		topicRestricted( false ),
	// 		keyEnabled( false ),
	// 		clientLimitEnabled( false ),
	// 		clientLimit( MAX_CLIENT_LIMIT ) 
	// {}
	Channel( );
	~Channel( );
	Channel( Channel const &src );
	Channel &operator=( Channel const &src );

	/** Setters */
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
	/** Getters */
	int GetInvitOnly(  );
	int GetTopic(  );
	int GetKey(  );
	int GetLimit(  );
	int GetClientsNumber();
	bool clientInChannel(std::string &nick);
	Client *get_client(int fd);
	Client *get_admin(int fd);
	Client *GetClientInChannel(std::string name);
	std::string GetTopicName();
	std::string GetPassword();
	std::string GetName();
	std::string GetTime();
	std::string get_creationtime();
	std::string getModes();
	bool getModeAtindex(size_t index) const;
	bool Gettopic_restriction() const;
	/** Methods */
	void add_client(Client newClient);
	void add_admin(Client newClient);
	void remove_client(int fd);
	void remove_admin(int fd);
	bool change_clientToAdmin(std::string& nick);
	bool change_adminToClient(std::string& nick);
	std::string clientChannel_list(  );
	void sendTo_all( std::string rpl1 );
	void sendTo_all( std::string rpl1, int fd );

};
