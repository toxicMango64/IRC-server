#include "../../inc/Server.hpp"

// static std::string Server::tTopic()
// {
// 	const std::time_t current = std::time(NULL);
// 	std::stringstream res;

// 	res << current;
// 	return res.str();
// }
// static std::string Server::gettopic(std::string &input)
// {
// 	const size_t pos = input.find(':');
// 	if (pos == std::string::npos)
// 	{
// 		return "";
// 	}
// 	return input.substr(pos);
// }

// static int Server::getpos(std::string &cmd)
// {
// 	for (int i = 0; i < (int)cmd.size(); i++) {
// 		if (cmd[i] == ':' && (cmd[i - 1] == 32)) {
// 			return i;
// 		}
// 	}
// 	return -1;
// }

static std::string tTopic()
{
	const std::time_t current = std::time(NULL);
	std::stringstream res;

	res << current;
	return res.str();
}

// static std::string gettopic(std::string &input)
// {
// 	const size_t pos = input.find(':');
// 	if (pos == std::string::npos)
// 	{
// 		return "";
// 	}
// 	return input.substr(pos);
// }

static int getpos(std::string &cmd)
{
	for (int i = 0; i < (int)cmd.size(); i++) {
		if (cmd[i] == ':' && (cmd[i - 1] == 32)) {
			return i;
		}
	}
	return -1;
}

void Server::Topic(std::string &cmd, int &fd)
{
	if (cmd == "TOPIC :") {
		senderror(461, GetClient(fd)->GetNickName(), fd, " :Not enough parameters\r\n");
		return;
	}
	std::vector<std::string> scmd = splitCmd(cmd);
	if (scmd.size() == 1) {
		senderror(461, GetClient(fd)->GetNickName(), fd, " :Not enough parameters\r\n");
		return ;
	}
	const std::string nmch = scmd[1].substr(1);
	if (GetChannel(nmch) == NULL) {
		senderror(403, "#" + nmch, fd, " :No such channel\r\n");
		return;
	}
	if ((GetChannel(nmch)->get_client(fd) == NULL) && (GetChannel(nmch)->get_admin(fd) == NULL)) {
		senderror(442, "#" + nmch, fd, " :You're not on that channel\r\n");
		return ;
	}
	if (scmd.size() == 2) {
		if (GetChannel(nmch)->GetTopicName() == "") {
			_sendResponse(": 331 " + GetClient(fd)->GetNickName() + " " + "#" + nmch + " :No topic is set\r\n", fd);
			return ;
		}
		const size_t pos = GetChannel(nmch)->GetTopicName().find(':');
		if (GetChannel(nmch)->GetTopicName() != "" && pos == std::string::npos) {
			_sendResponse(": 332 " + GetClient(fd)->GetNickName() + " " + "#" + nmch + " " + GetChannel(nmch)->GetTopicName() + "\r\n", fd);
			_sendResponse(": 333 " + GetClient(fd)->GetNickName() + " " + "#" + nmch + " " + GetClient(fd)->GetNickName() + " " + GetChannel(nmch)->GetTime() + "\r\n", fd);
			return ;
		}
		else {
			const size_t pos = GetChannel(nmch)->GetTopicName().find(' ');
			if (pos == 0) {
				GetChannel(nmch)->GetTopicName().erase(0, 1);
			}
			_sendResponse(": 332 " + GetClient(fd)->GetNickName() + " " + "#" + nmch + " " + GetChannel(nmch)->GetTopicName() + "\r\n", fd);
			_sendResponse(": 333 " + GetClient(fd)->GetNickName() + " " + "#" + nmch + " " + GetClient(fd)->GetNickName() + " " + GetChannel(nmch)->GetTime() + "\r\n", fd);
			return ;
		}
	}

	if (scmd.size() >= 3) {
		std::vector<std::string> tmp;
		const int pos = getpos(cmd);
		if (pos == -1 || scmd[2][0] != ':') {
			tmp.push_back(scmd[0]);
			tmp.push_back(scmd[1]);
			tmp.push_back(scmd[2]);
		}
		else {
			tmp.push_back(scmd[0]);
			tmp.push_back(scmd[1]);
			tmp.push_back(cmd.substr(pos));
		}

		if (tmp[2][0] == ':' && tmp[2][1] == '\0') {
			senderror(331, "#" + nmch, fd, " :No topic is set\r\n");
			return ;
		}

		if (GetChannel(nmch)->Gettopic_restriction() == true && GetChannel(nmch)->get_client(fd) != NULL) {
			senderror(482, "#" + nmch, fd, " :You're Not a channel operator\r\n");
			return ;
		}
		else if (GetChannel(nmch)->Gettopic_restriction() == true && GetChannel(nmch)->get_admin(fd) != NULL) {
			GetChannel(nmch)->SetTime(tTopic());
			GetChannel(nmch)->SetTopicName(tmp[2]);
			std::string rpl;
			const size_t pos = tmp[2].find(':');
			if (pos == std::string::npos) {
				rpl = ":" + GetClient(fd)->GetNickName() + "!" + GetClient(fd)->GetUserName() + "@localhost TOPIC #" + nmch + " :" + GetChannel(nmch)->GetTopicName() + "\r\n";
            }
			else {
				rpl = ":" + GetClient(fd)->GetNickName() + "!" + GetClient(fd)->GetUserName() + "@localhost TOPIC #" + nmch + " " + GetChannel(nmch)->GetTopicName() + "\r\n";
            }
			GetChannel(nmch)->sendTo_all(rpl);
		}
		else {
			std::string rpl;
			const size_t pos = tmp[2].find(':');
			if (pos == std::string::npos) {
				GetChannel(nmch)->SetTime(tTopic());
				GetChannel(nmch)->SetTopicName(tmp[2]);
				rpl = ":" + GetClient(fd)->GetNickName() + "!" + GetClient(fd)->GetUserName() + "@localhost TOPIC #" + nmch + " " + GetChannel(nmch)->GetTopicName() + "\r\n";
			}
			else {
				const size_t poss = tmp[2].find(' ');
				GetChannel(nmch)->SetTopicName(tmp[2]);
				if (poss == std::string::npos && tmp[2][0] == ':' && tmp[2][1] != ':') {
					tmp[2] = tmp[2].substr(1);
				}
				GetChannel(nmch)->SetTopicName(tmp[2]);
				GetChannel(nmch)->SetTime(tTopic());
				rpl = ":" + GetClient(fd)->GetNickName() + "!" + GetClient(fd)->GetUserName() + "@localhost TOPIC #" + nmch + " " + GetChannel(nmch)->GetTopicName() + "\r\n";
			}
			GetChannel(nmch)->sendTo_all(rpl);
		}
	}
}
