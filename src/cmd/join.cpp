#include "../../inc/Server.hpp"
#include "../../inc/Response.hpp"

bool isValidChannelName(const std::string& name) {
    if (name.size() < 2) return false; // must be at least 2 characters
    if (name[0] != '#' && name[0] != '&') return false;
    for (size_t i = 0; i < name.size(); ++i) {
        if (name[i] == ' ' || name[i] == ',' || name[i] == 7)
            return false;
    }
    if (name.size() > 200) return false;
    return true;
}


int Server::SplitJoin(std::vector<std::pair<std::string, std::string> > &token, const std::string& cmd, int fd)
{
    std::vector<std::string> tmp;
    std::string ChStr;
    std::string PassStr;
    std::string buff;
    std::istringstream iss(cmd);
    while (iss >> ChStr) {
        tmp.push_back(ChStr);
    }

    if (tmp.size() < 2) {
        token.clear();
        return 0;
    }

    tmp.erase(tmp.begin());
    ChStr = tmp[0];
    tmp.erase(tmp.begin());

    if (!tmp.empty()) {
        PassStr = tmp[0];
        tmp.clear();
    }

    for (size_t i = 0; i < ChStr.size(); i++) {
        if (ChStr[i] == ',') {
            token.push_back(std::make_pair(buff, ""));
            buff.clear();
        } else {
            buff += ChStr[i];
        }
    }
    token.push_back(std::make_pair(buff, ""));

    if (!PassStr.empty()) {
        size_t j = 0;
        buff.clear();
        for (size_t i = 0; i < PassStr.size(); i++) {
            if (PassStr[i] == ',') {
                token[j].second = buff;
                j++;
                buff.clear();
            } else {
                buff += PassStr[i];
            }
        }
        token[j].second = buff;
    }

    for (size_t i = 0; i < token.size(); i++) {
        if (token[i].first.empty()) {
            token.erase(token.begin() + i);
            --i;
        }
    }

    for (size_t i = 0; i < token.size(); i++) {
        if (!isValidChannelName(token[i].first)) {
            senderror(476, GetClient(fd)->GetNickName(), token[i].first, GetClient(fd)->GetFd(), " :Invalid channel name\r\n");
            token.erase(token.begin() + i);
            --i;
        } else {
            // token[i].first.erase(token[i].first.begin());
        }
    }

    return 1;
}

int Server::SearchForClients(const std::string& nickname)
{
    int count = 0;
    for (size_t i = 0; i < this->channels.size(); i++) {
        if (this->channels[i].GetClientInChannel(nickname) != NULL) {
            count++;
        }
    }
    return count;
}

bool IsInvited(Client* cli, std::string ChName, int flag)
{
    if (cli->GetInviteChannel(ChName)) {
        if (flag == 1) {
            cli->RmChannelInvite(ChName);
        }
        return true;
    }
    return false;
}

void Server::ExistCh(std::vector<std::pair<std::string, std::string> >& token, size_t i, size_t j, int fd)
{
    if (this->channels[j].GetClientInChannel(GetClient(fd)->GetNickName()) != NULL) {
        return;
    }

    if (SearchForClients(GetClient(fd)->GetNickName()) >= 10) {
        senderror(405, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :You have joined too many channels\r\n");
        return;
    }

    if (!this->channels[j].GetPassword().empty() && this->channels[j].GetPassword() != token[i].second) {
        if (!IsInvited(GetClient(fd), token[i].first, 0)) {
            senderror(475, GetClient(fd)->GetNickName(), token[i].first, GetClient(fd)->GetFd(), " :Cannot join channel (+k) - bad key\r\n");
            return;
        }
    }

    if (this->channels[j].GetInvitOnly()) {
        if (!IsInvited(GetClient(fd), token[i].first, 1)) {
            senderror(473, GetClient(fd)->GetNickName(), token[i].first, GetClient(fd)->GetFd(), " :Cannot join channel (+i)\r\n");
            return;
        }
    }

    if (this->channels[j].GetLimit() && this->channels[j].GetClientsNumber() >= this->channels[j].GetLimit()) {
        senderror(471, GetClient(fd)->GetNickName(), token[i].first, GetClient(fd)->GetFd(), " :Cannot join channel (+l)\r\n");
        return;
    }

    Client* cli = GetClient(fd);
    this->channels[j].add_client(*cli);

    if (channels[j].GetTopicName().empty()) {
        _sendResponse(RPL_JOINMSG(GetClient(fd)->getHostname(), GetClient(fd)->getIpAdd(), token[i].first) +
                      RPL_NAMREPLY(GetClient(fd)->GetNickName(), channels[j].GetName(), channels[j].clientChannel_list()) +
                      RPL_ENDOFNAMES(GetClient(fd)->GetNickName(), channels[j].GetName()), fd);
    } else {
        _sendResponse(RPL_JOINMSG(GetClient(fd)->getHostname(), GetClient(fd)->getIpAdd(), token[i].first) +
                      RPL_TOPICIS(GetClient(fd)->GetNickName(), channels[j].GetName(), channels[j].GetTopicName()) +
                      RPL_NAMREPLY(GetClient(fd)->GetNickName(), channels[j].GetName(), channels[j].clientChannel_list()) +
                      RPL_ENDOFNAMES(GetClient(fd)->GetNickName(), channels[j].GetName()), fd);
    }

    channels[j].sendTo_all(RPL_JOINMSG(GetClient(fd)->getHostname(), GetClient(fd)->getIpAdd(), token[i].first), fd);
}

void Server::NotExistCh(std::vector<std::pair<std::string, std::string> >& token, size_t i, int fd)
{
    if (SearchForClients(GetClient(fd)->GetNickName()) >= 10) {
        senderror(405, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :You have joined too many channels\r\n");
        return;
    }

    Channel newChannel;
    newChannel.SetName(token[i].first);
    newChannel.add_admin(*GetClient(fd));
    newChannel.set_createiontime();
    this->channels.push_back(newChannel);

    _sendResponse(RPL_JOINMSG(GetClient(fd)->getHostname(), GetClient(fd)->getIpAdd(), newChannel.GetName()) +
                  RPL_NAMREPLY(GetClient(fd)->GetNickName(), newChannel.GetName(), newChannel.clientChannel_list()) +
                  RPL_ENDOFNAMES(GetClient(fd)->GetNickName(), newChannel.GetName()), fd);
}

void Server::JOIN(const std::string& cmd, int fd)
{
    std::vector<std::pair<std::string, std::string > > token;

    if (!SplitJoin(token, cmd, fd)) {
        senderror(461, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :Not enough parameters\r\n");
        return;
    }

    if (token.size() > 10) {
        senderror(407, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :Too many channels\r\n");
        return;
    }

    for (size_t i = 0; i < token.size(); i++) {
        bool flag = false;
        for (size_t j = 0; j < this->channels.size(); j++) {
            if (this->channels[j].GetName() == token[i].first) {
                ExistCh(token, i, j, fd);
                flag = true;
                break;
            }
        }

        if (!flag) {
            NotExistCh(token, i, fd);
        }
    }
}
