#include "../../inc/Server.hpp"

void Server::Invite( std::string &cmd, int &fd )
{
	std::vector<std::string> scmd = split_cmd(cmd);
	if(scmd.size() < 3) {
	senderror(461, GetClient(fd)->GetNickName(), fd, " :Not enough parameters\r\n");
		return;
	}
	std::string channelname = scmd[2].substr(1);
	if(scmd[2][0] != '#' || !GetChannel(channelname)) {
		senderror(403, channelname, fd, " :No such channel\r\n");
		return;
	}
	if (!(GetChannel(channelname)->get_client(fd)) && !(GetChannel(channelname)->get_admin(fd))) {
		senderror(442, channelname, fd, " :You're not on that channel\r\n");
		return;
	}
	if (GetChannel(channelname)->GetClientInChannel(scmd[1])) {
		senderror(443, GetClient(fd)->GetNickName(), channelname, fd, " :is already on channel\r\n");
		return;
	}
	Client *clt = GetClientNick(scmd[1]);
	if (!clt) {
		senderror(401, scmd[1], fd, " :No such nick\r\n");
		return;
	}
	if (GetChannel(channelname)->GetInvitOnly() && !GetChannel(channelname)->get_admin(fd)) {
		senderror(482,GetChannel(channelname)->get_client(fd)->GetNickName(),scmd[1],fd," :You're not channel operator\r\n");
		return;
	}
	if (GetChannel(channelname)->GetLimit() && GetChannel(channelname)->GetClientsNumber() >= GetChannel(channelname)->GetLimit()) {
		senderror(473,GetChannel(channelname)->get_client(fd)->GetNickName(),channelname,fd," :Cannot invit to channel (+i)\r\n");
		return;
	}
	clt->AddChannelInvite(channelname);
	const std::string rep1 = ": 341 "+ GetClient(fd)->GetNickName()+" "+ clt->GetNickName()+" "+ scmd[2]+"\r\n";
	_sendResponse(rep1, fd);
	const std::string rep2 = ":"+ clt->getHostname() + " INVITE " + clt->GetNickName() + " " + scmd[2]+"\r\n";
	_sendResponse(rep2, clt->GetFd());
	
}
