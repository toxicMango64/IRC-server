#pragma once

#include <string>
#include <set>
#include <vector>
#include <iostream>

enum ClientState {
    UNAUTHENTICATED,
    AUTHENTICATED,
    REGISTERED,
};

class Client {
	public:
		Client();
		Client( int fd );
		Client(std::string nickname, std::string username, int fd);
		~Client();

		Client(Client const &src);
		Client &operator=(Client const &src);

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
		std::string		getHostname();
		

		void	SetFd(int fd);
		void	SetNickname(std::string& nickName);
		void	setLogedin(bool value);
		void	SetUsername(std::string& username);
		void	setBuffer(std::string recived);
		void	setRegistered(bool value);
		void	setIpAdd(std::string ipadd);


	private:
		int                     fd;
		std::string             nickname;
		std::string             realname;
		std::string             username;
		ClientState             state;
		std::set<std::string>   channels;
		bool                    isOperator;
		bool 					registered;
		std::string				buffer;
		std::string				ipadd;
		bool					logedin;
		
		std::vector<std::string>	ChannelsInvite;

};
