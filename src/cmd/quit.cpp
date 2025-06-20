#include "../../inc/Server.hpp"
#include <iostream>  // for std::cout
#include <sstream>   // for std::istringstream

void FindQ(const std::string& cmd, const std::string& to_find, std::string& str)
{
	size_t i = 0;
	while (i < cmd.size())
	{
		if (cmd[i] != ' ')
		{
			std::string tmp;
			for (; i < cmd.size() && cmd[i] != ' '; ++i) {
				tmp += cmd[i];
			}
			if (tmp == to_find) {
				break;
			}
			else {
				tmp.clear();
			}
		}
		else {
			++i;
		}
	}
	if (i < cmd.size()) {
		str = cmd.substr(i);
	}
	else {
		str.clear();
	}
	i = 0;
	while (i < str.size() && str[i] == ' ') {
		++i;
	}
	str = str.substr(i);
}

std::string SplitQuit(const std::string& cmd)
{
	std::istringstream stm(cmd);
	std::string reason;
	std::string str;
	stm >> str;

	FindQ(cmd, str, reason);

	if (reason.empty()) {
		return "Quit";
	}

	if (reason[0] != ':') {
		for (size_t i = 0; i < reason.size(); ++i) {
			if (reason[i] == ' ') {
				reason.erase(reason.begin() + static_cast<std::ptrdiff_t>(i), reason.end());
				break ;
			}
		}
		reason.insert(reason.begin(), ':');
	}
	return reason;
}

void Server::QUIT(const std::string& cmd, int fd) {
	const std::string reason = SplitQuit(cmd);

	for (size_t i = 0; i < channels.size(); /* increment inside */) {
		if (channels[i].get_client(fd) != NULL) {
			channels[i].remove_client(fd);
			if (channels[i].GetClientsNumber() == 0) {
				channels.erase(channels.begin() + static_cast<std::ptrdiff_t>(i));
				continue ;
			}
			else {
				const std::string rpl = ":" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + "@server QUIT " + reason + "\r\n";
				channels[i].sendToAll(rpl);
			}
		}
		else if (channels[i].get_admin(fd) != NULL) {
			channels[i].remove_admin(fd);
			if (channels[i].GetClientsNumber() == 0) {
				channels.erase(channels.begin() + static_cast<std::ptrdiff_t>(i));
				continue ;
			}
			else {
				const std::string rpl = ":" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + "@server QUIT " + reason + "\r\n";
				channels[i].sendToAll(rpl);
			}
		}
		++i;
	}

	std::cout << "Client <" << fd << "> Disconnected" << std::endl;

	RmChannels(fd);
	RemoveClient(fd);
	RemoveFds(fd);
	close(fd);
}
