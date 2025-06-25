#pragma once

#if defined(_MSC_VER)
  #ifdef BUILDING_MYLIB
    #define EXPORT __declspec(dllexport)
  #else
    #define EXPORT __declspec(dllimport)
  #endif
#elif defined(__GNUC__) || defined(__clang__)
  #define EXPORT __attribute__((visibility("default")))
#else
  #define EXPORT
#endif

#include <stdexcept>
#include <stdint.h>
#include <iostream>
#include "Utils.hpp"

#include <cstring>
#include <map>
#include <string>
#include <vector>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "Client.hpp"
#include "Channel.hpp"
#include "CommandHandler.hpp"


enum e_irc {
	MIN_PORT = 0,
	MAX_PORT = 65535,
};

class Client;
class Channel;

class Server {
private:
	int _port;
	std::string _password;
	static bool _signalRecvd;

	int							sfds;
	std::vector<Client>			clients;
	std::vector<Channel>		channels;
	std::vector<struct pollfd>	fds;
	struct sockaddr_in			add;
	struct sockaddr_in			cliaddr;
	struct pollfd				new_cli;
	std::string 				serverName;

public:
	// static const int	MAX_BUF = 512;
	// static char	buffer[MAX_BUF] __attribute__((aligned(16)));

	bool				isValid( void ) const;
	static void			signalHandler(int signum);

	Server();
	~Server();
	Server( Server const &src );
	Server &operator=( Server const &src );
	Server( int port, const std::string& password );

	void	run( int sFd );
	void	closeFds( void );
	int		createSocket( void );
	void	bindSocket( int sFd );
	int		setNonBlocking( int fd );
	void	startListening( int sFd );
	void	handleClientMessage( size_t i );
	void	handleNewConnection( int sFd );
	void	handleClientWrite( size_t clientIndex ); // New method for POLLOUT

	int	GetPort( );
	int	GetFd( );
	const int&	getPort( ) const { return _port; }
	Client	*GetClient( int fd );
	Client	*GetClientNick( std::string nickname );
	Channel	*GetChannel( std::string name );
	std::string	GetPassword( );

	void	SetFd( int sfds );
	void	SetPort( int port );
	void	SetPassword( std::string password );
	void	AddClient( Client newClient );
	void	AddChannel( Channel newChannel );
	void	AddFds( pollfd newFd );
	void	set_username( std::string& username, int fd );
	void	set_nickname( std::string cmd, int fd );

	void	RemoveClient( int fd );
	void	RemoveChannel( std::string name );
	void	RemoveFds( int fd );
	void	RmChannels( int fd );

	void	senderror( int code, std::string clientname, int fd, std::string msg );
	void	senderror( int code, std::string clientname, std::string channelname, int fd, std::string msg );
	void	_sendResponse( std::string response, int fd );


	void	getCmd( std::string &cmd, int fd );
	std::vector<std::string>	splitCmd( std::string &str );

	bool nickNameInUse( std::string& nickname );
	void client_authen( int fd, std::string pass );

	void	JOIN(const std::string& cmd, std::vector<std::string> tokens, int fd);
	int		SplitJoin(std::vector<std::pair<std::string, std::string> > &token, const std::string& cmd, int fd);
	void	ExistCh(std::vector<std::pair<std::string, std::string> >& token, size_t i, size_t j, int fd);
	void	NotExistCh(std::vector<std::pair<std::string, std::string> >& token, size_t i, int fd);
	int		SearchForClients(const std::string& nickname);

	void	PART(const std::string& cmd, int fd);
	int		SplitCmdPart(const std::string& cmd, std::vector<std::string> &tmp, std::string &reason, int fd);

	void	KICK(const std::string &cmd, int fd);
	std::string SplitCmdKick(const std::string &cmd, std::vector<std::string> &tmp, std::string &user, int fd);

	void	PRIVMSG(const std::string& cmd, int fd);
	void	CheckForChannels_Clients( std::vector<std::string> &tmp, int fd );

	void	QUIT(const std::string& cmd, int fd);

	void 		mode_command( std::string& cmd, int fd );
	std::string invite_only(Channel* channel, char opera, const std::string& chain);
	std::string topic_restriction(Channel* channel, char opera, const std::string& chain);
	std::string password_mode(const std::vector<std::string>& tokens, Channel* channel, size_t& pos, char opera, int fd, const std::string& chain, std::string& arguments);
	std::string operator_privilege(const std::vector<std::string>& tokens, Channel* channel, size_t& pos, int fd, char opera, const std::string& chain, std::string& arguments);
	std::string channel_limit(const std::vector<std::string>& tokens, Channel* channel, size_t& pos, char opera, int fd, const std::string& chain, std::string& arguments);
	bool	isvalid_limit(const std::string& limit);
	std::string	mode_toAppend(const std::string& chain, char opera, char mode);
	std::vector<std::string>	splitParams( std::string params );
	void	getCmdArgs(const std::string& cmd, std::string& name, std::string& modeset, std::string& params);

	void Topic( std::string &cmd, int &fd );
	void Invite( std::string &cmd, int &fd );
};
