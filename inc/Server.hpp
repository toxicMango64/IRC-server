// Server.hpp
#pragma once

#include <stdexcept>
#include <stdint.h>
#include <iostream>
#include "Utils.hpp"

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
#include "Response.hpp"

// enum class e_irc : std::uint16_t {
enum e_irc {
	MIN_PORT = 0,
	MAX_PORT = 65535,
	MAX_INT_INDEX = 10
};

// #include <stdio.h>

// #define welcome(server) printf("welcome to {%s}, hope you enjoy your time here\n", server);

class Client;
class Channel;

class Server {
private:
	// static const int MAX_BUF = 512;
	int _port;
	std::string _password;
	static bool Signal;

	int							sfds;
	std::map<int, Client>		connectedClients;
	std::vector<Client>			clients;
	std::vector<Channel>		channels;
	std::vector<struct pollfd>	fds;
	struct sockaddr_in			add;
	struct sockaddr_in			cliaddr;
	struct pollfd				new_cli;

public:
	static const int	MAX_BUF = 512;
	static const char	buffer[MAX_BUF] __attribute__((aligned(16)));

	bool				isValid( void ) const;
	static void			signalHandler(int signum);

	Server();
	~Server();
	Server( Server const &src );
	Server &operator=( Server const &src );
	Server( int port, const std::string& password );

	void	run( int sFd );
	void	closeFds( void ); /** implement the fucntion */
	static void SignalHandler( int signum );
	int		createSocket( void );
	void	bindSocket( int sFd );
	int		setNonBlocking( int fd );
	void	startListening( int sFd );
	void	handleClientMessage( size_t i, std::vector<pollfd>& fds );
	void	handleNewConnection( int sFd, std::vector<pollfd>& fds );

	// static bool	isBotfull;
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

	void	init( int port, std::string pass );
	void	accept_new_client( );
	void	set_sever_socket( );
	void	reciveNewData( int fd );

	void	parse_exec_cmd( std::string &cmd, int fd );
	std::vector<std::string>	split_recivedBuffer( std::string str );
	std::vector<std::string>	split_cmd( std::string &str );

	bool BypassForBot( std::string cmd, int fd );
	bool notregistered( int fd );
	bool nickNameInUse( std::string& nickname );
	bool is_validNickname( std::string& nickname );
	void client_authen( int fd, std::string pass );

	void	JOIN( std::string cmd, int fd );
	int		SplitJoin( std::vector<std::pair<std::string, std::string> > &token, std::string cmd, int fd );
	void	ExistCh( std::vector<std::pair<std::string, std::string> >&token, int i, int j, int fd );
	void	NotExistCh( std::vector<std::pair<std::string, std::string> >&token, int i, int fd );
	int		SearchForClients( std::string nickname );

	void	PART( std::string cmd, int fd );
	int		SplitCmdPart( std::string cmd, std::vector<std::string> &tmp, std::string &reason, int fd );

	void	KICK( std::string cmd, int fd );
	std::string SplitCmdKick( std::string cmd, std::vector<std::string> &tmp, std::string &user, int fd );

	void	PRIVMSG( std::string cmd, int fd );
	void	CheckForChannels_Clients( std::vector<std::string> &tmp, int fd );

	void	QUIT( std::string cmd, int fd );

	void 		mode_command( std::string& cmd, int fd );
	std::string invite_only( Channel *channel, char opera, std::string chain );
	std::string topicRestricted( Channel *channel ,char opera, std::string chain );
	std::string password_mode( std::vector<std::string> splited, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments );
	std::string operator_privilege( std::vector<std::string> splited, Channel *channel, size_t& pos, int fd, char opera, std::string chain, std::string& arguments );
	std::string channel_limit( std::vector<std::string> splited, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string& arguments );
	bool	isvalid_limit( std::string& limit );
	std::string	mode_toAppend( std::string chain, char opera, char mode );
	std::vector<std::string>	splitParams( std::string params );
	void	getCmdArgs( std::string cmd,std::string& name, std::string& modeset ,std::string &params );

	std::string tTopic(  );
	void Topic( std::string &cmd, int &fd );
	void Invite( std::string &cmd, int &fd );
	std::string gettopic( std::string& input );
	int getpos( std::string &cmd );

};
