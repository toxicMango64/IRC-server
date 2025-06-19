// Client.hpp
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

		/** getters */
		int				GetFd();
		ClientState		getState() const; // New getter for ClientState
		bool			GetInviteChannel(std::string &ChName);
		std::string		GetNickName();
		std::string		GetUserName();
		std::string		getIpAdd();
		std::string		getBuffer();
		std::string		getHostname();
		
		/** setters */
		void	SetFd(int fd);
		void	SetNickname(std::string& nickName);
		void	SetUsername(std::string& username);
		void	setBuffer(std::string recived);
		void	setState(ClientState newState); // New setter for ClientState
		void	setIpAdd(std::string ipadd);


	private:
		int                     fd;
		std::string             nickname;
		std::string             realname;
		std::string             username;
		ClientState             state;
		std::set<std::string>   channels;
		bool                    isOperator;
		std::string				buffer;
		std::string				ipadd;
		
		std::vector<std::string>	ChannelsInvite;

};
