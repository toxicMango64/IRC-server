#include "CommandHandler.hpp"
#include "Response.hpp"

std::vector<std::string> splitReceivedBuffer(const std::string& str) {
	std::vector<std::string> vec;
	std::istringstream stm(str);
	std::string line;
	while (std::getline(stm, line)) {
		const size_t pos = line.find_first_of("\r\n");
		if (pos != std::string::npos)
			line = line.substr(0, pos);
		vec.push_back(line);
	}
	return vec;
}

std::vector<std::string> Server::splitCmd(std::string& cmd)
{
	std::vector<std::string> vec;
	std::istringstream stm(cmd);
	std::string token;
	while(stm >> token)
	{
		vec.push_back(token);
		token.clear();
	}
	return vec;
}

void Server::client_authen(int fd, std::string cmd)
{
	Client *cli = GetClient(fd);
	cmd = cmd.substr(4);
	const size_t pos = cmd.find_first_not_of("\t\v ");
	if(pos < cmd.size())
	{
		cmd = cmd.substr(pos);
		if(cmd[0] == ':')
			cmd.erase(cmd.begin());
	}
	if (pos == std::string::npos || cmd.empty()) 
		_sendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
	else if (!cli->getRegistered())
	{
		const std::string& pass = cmd;
		if (pass == _password) {
			cli->setRegistered(true);
		}
		else
			_sendResponse(ERR_INCORPASS(std::string("*")), fd);
	}
	else
		_sendResponse(ERR_ALREADYREGISTERED(GetClient(fd)->GetNickName()), fd);
}

bool isValidNickname(const std::string& nickname) {
    if (nickname.empty() || nickname.length() > 9) {
        return false;
    }
    if (!std::isalpha(nickname[0])) {
        return false;
    }
    for (size_t i = 1; i < nickname.length(); ++i) {
        if (!std::isalnum(nickname[i]) && nickname[i] != '-' && nickname[i] != '[' && nickname[i] != ']' && nickname[i] != '\\' && nickname[i] != '`' && nickname[i] != '^' && nickname[i] != '{' && nickname[i] != '}') {
            return false;
        }
    }
    return true;
}

bool Server::nickNameInUse(std::string& nickname) {
	for (size_t i = 0; i < this->clients.size(); i++)
	{
		if (this->clients[i].GetNickName() == nickname)
			return true;
	}
	return false;
}

void Server::set_nickname(std::string cmd, int fd) {
	std::string inuse;
	cmd = cmd.substr(4);
	const size_t pos = cmd.find_first_not_of("\t\v ");
	if (pos < cmd.size()) {
		cmd = cmd.substr(pos);
		if (cmd[0] == ':') {
			cmd.erase(cmd.begin());
		}
	}
	Client *cli = GetClient(fd);
	if(pos == std::string::npos || cmd.empty()) {
		_sendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
		return;
	}
	if (nickNameInUse(cmd) && cli->GetNickName() != cmd) {
		inuse = "*";
		if (cli->GetNickName().empty()) {
			cli->SetNickname(inuse);
		}
	    _sendResponse(ERR_NICKINUSE(std::string(cmd)), fd); 
		return ;
	}
	if(!isValidNickname(cmd)) {
		_sendResponse(ERR_ERRONEUSNICK(std::string(cmd)), fd);
		return ;
	}
	else {
		if (cli && cli->getRegistered()) {
			const std::string oldnick = cli->GetNickName();
			cli->SetNickname(cmd);
			for(size_t i = 0; i < channels.size(); i++) {
				Client *cl = channels[i].GetClientInChannel(oldnick);
				if (cl) {
					cl->SetNickname(cmd);
				}
			}
			if(!oldnick.empty() && oldnick != cmd) {
				if (oldnick == "*" && !cli->GetUserName().empty()) {
					cli->setLogedin(true);
					_sendResponse(RPL_CONNECTED(cli->GetNickName()), fd);
					_sendResponse(RPL_YOURHOST(cli->GetNickName()), fd);
					_sendResponse(RPL_CREATED(cli->GetNickName()), fd);
					_sendResponse(RPL_MYINFO(cli->GetNickName()), fd);
					_sendResponse(RPL_ISUPPORT(cli->GetNickName()), fd);
				}
				else {
					_sendResponse(RPL_NICKCHANGE(cli->getHostname(oldnick), cmd), fd);
				}
				return ;
			}
			
		}
		else if (cli && !cli->getRegistered()) {
			_sendResponse(ERR_NOTREGISTERED("*"), fd);
		}
	}
	if(cli && cli->getRegistered() && !cli->GetUserName().empty() && !cli->GetNickName().empty() && cli->GetNickName() != "*" && !cli->GetLogedIn()) {
		cli->setLogedin(true);
		_sendResponse(RPL_CONNECTED(cli->GetNickName()), fd);
		_sendResponse(RPL_YOURHOST(cli->GetNickName()), fd);
		_sendResponse(RPL_CREATED(cli->GetNickName()), fd);
		_sendResponse(RPL_MYINFO(cli->GetNickName()), fd);
		_sendResponse(RPL_ISUPPORT(cli->GetNickName()), fd);
	}
}

void	Server::set_username(std::string& cmd, int fd)
{
	std::vector<std::string> splited_cmd = splitCmd(cmd);

	Client *cli = GetClient(fd); 
	if((cli && splited_cmd.size() < 5)) {
		_sendResponse(ERR_NOTENOUGHPARAM(cli->GetNickName()), fd);
		return;
	}
	if(!cli  || !cli->getRegistered()) {
		_sendResponse(ERR_NOTREGISTERED(std::string("*")), fd);
	}
	else if (cli && !cli->GetUserName().empty()) {
		_sendResponse(ERR_ALREADYREGISTERED(cli->GetNickName()), fd);
		return;
	}
	else {
		cli->SetUsername(splited_cmd[1]);
	}
	if((cli != 0) && cli->getRegistered() && !cli->GetUserName().empty() && !cli->GetNickName().empty() && cli->GetNickName() != "*"  && !cli->GetLogedIn())
	{
		cli->setLogedin(true);
		_sendResponse(RPL_CONNECTED(cli->GetNickName()), fd);
		_sendResponse(RPL_YOURHOST(cli->GetNickName()), fd);
		_sendResponse(RPL_CREATED(cli->GetNickName()), fd);
		_sendResponse(RPL_MYINFO(cli->GetNickName()), fd);
		_sendResponse(RPL_ISUPPORT(cli->GetNickName()), fd);
	}
}
