#include "../../inc/Server.hpp"

void Server::Invite( std::string &cmd, int &fd )
{
    std::vector<std::string> scmd = splitCmd(cmd);
    if(scmd.size() < 3) {
        senderror(461, GetClient(fd)->GetNickName(), fd, " :Not enough parameters\r\n");
        return ;
    }
    // std::string channelname = scmd[2].substr(1); ///////////////////////////////////////////////////////////this got changed after yumna's evaluation
    // if(scmd[2][0] != '#' || GetChannel(channelname) == NULL) {
    //     senderror(403, channelname, fd, " :No such channel\r\n");
    //     return ;
    // }
    std::string channelname = scmd[2].substr(1); ///////////////////////////////////////////////////////////this got changed after yumna's evaluation
    if(scmd[2][0] != '#' || GetChannel(channelname) == NULL) {
        senderror(403, channelname, fd, " :No such channel\r\n");
        return ;
    }

    if (GetChannel(channelname)->get_client(fd) == NULL && GetChannel(channelname)->get_admin(fd) == NULL) {
        senderror(442, channelname, fd, " :You're not on that channel\r\n");
        return ;
    }

    if (GetChannel(channelname)->GetClientInChannel(scmd[1]) != NULL) {
        senderror(443, GetClient(fd)->GetNickName(), channelname, fd, " :is already on channel\r\n");
        return ;
    }

    Client *clt = GetClientNick(scmd[1]);
    if (clt == NULL) {
        senderror(401, scmd[1], fd, " :No such nick\r\n");
        return ;
    }

    if (GetChannel(channelname)->GetInvitOnly() != 0 && GetChannel(channelname)->get_admin(fd) == NULL) {
        senderror(482, GetChannel(channelname)->get_client(fd)->GetNickName(), scmd[1], fd, " :You're not channel operator\r\n");
        return ;
    }

    int clientNumber = GetChannel(channelname)->GetClientsNumber();
    if (clientNumber == -1) {
        senderror(473, GetClient(fd)->GetNickName(), channelname, fd, " :Cannot invite to channel\r\n");
        return ;
    }
    if (GetChannel(channelname)->GetLimit() != 0 && clientNumber >= GetChannel(channelname)->GetLimit()) {
        senderror(473, GetClient(fd)->GetNickName(), channelname, fd, " :Cannot invite to channel\r\n");
        return ;
    }

    clt->AddChannelInvite(channelname);
    const std::string rep1 = ": 341 "+ GetClient(fd)->GetNickName()+" "+ clt->GetNickName()+" "+ scmd[2]+"\r\n";
    _sendResponse(rep1, fd);
    const std::string rep2 = ":"+ clt->getHostname() + " INVITE " + clt->GetNickName() + " " + scmd[2]+"\r\n";
    _sendResponse(rep2, clt->GetFd());
}
