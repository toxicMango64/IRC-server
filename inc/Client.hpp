#pragma once

#include <string>
#include <set>
#include <vector>
#include <iostream>

#ifndef MAX_BUF
# define MAX_BUF 512
#endif

class Client {
	public:
		Client();
		Client( int fd );
		Client( const std::string& nickname, const std::string& username, int fd );
		Client(Client const &src);
		Client &operator=(Client const &src);
		~Client();

		void	clearBuffer();
		void	AddChannelInvite(std::string &chname);
		void	RmChannelInvite(std::string &chname);
		void	handleBuffer(Client& client, const char buf[512], const std::string& password, std::string& output);


		int				GetFd();
		bool			getRegistered();
		bool			GetInviteChannel(std::string &ChName);
		std::string		GetNickName();
		bool			GetLogedIn();
		std::string		GetUserName();
		std::string		getIpAdd();
		std::string		getBuffer();
		std::string		getHostname( const std::string& oldNick );
		std::string		getHostname();
		std::string&	getOutgoingBuffer(); // New method
		

		void	SetFd(int fd);
		void	SetNickname(std::string& nickName);
		void	setLogedin(bool value);
		void	SetUsername(std::string& username);
		void	setBuffer(std::string recived);
		void	setRegistered(bool value);
		void	setIpAdd(std::string ipadd);
		void	appendOutgoing(const std::string& data); // New method
		void	clearOutgoingBuffer(size_t bytesSent); // New method


	private:
		int                     fd;
		std::string             nickname;
		std::string             realname;
		std::string             username;
		std::set<std::string>   channels;
		bool                    isOperator;
		bool 					registered;
		std::string				ipadd;
		bool					logedin;
		// std::string				buffer;
		static char	buffer[MAX_BUF]	__attribute__((aligned(16)));
		std::string				_outgoingBuffer; // Buffer for data to be sent
		std::vector<std::string>	ChannelsInvite;
};
