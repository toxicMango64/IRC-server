#include "../../inc/Server.hpp"
#include "../../inc/Response.hpp"
#include <cstdlib>

std::string Server::mode_toAppend(const std::string& chain, char opera, char mode) {
    char last = '\0';

    for (size_t i = 0; i < chain.length(); ++i) {
        char c = chain[i];
        if (c == '+' || c == '-')
            last = c;
    }

    if (last != opera)
        return std::string(1, opera) + mode;
    else
        return std::string(1, mode);
}

void Server::getCmdArgs(const std::string& cmd, std::string& name, std::string& modeset, std::string& params) {
    std::istringstream stm(cmd);
    stm >> name >> modeset;

    size_t found = cmd.find_first_not_of(name + modeset + " \t\v");
    if (found != std::string::npos)
        params = cmd.substr(found);
}

std::vector<std::string> Server::splitParams(std::string params)
{
    if (!params.empty() && params[0] == ':')
        params.erase(params.begin());

    std::vector<std::string> tokens;
    std::string token;
    std::istringstream stm(params);
    while (std::getline(stm, token, ','))
    {
        tokens.push_back(token);
    }
    return tokens;
}

void Server::mode_command(std::string& cmd, int fd)
{
    std::string channelName, params, modeset;
    std::stringstream mode_chain;
    std::string arguments;
    Channel* channel = NULL;
    char opera = '\0';

    Client* cli = GetClient(fd);

    size_t found = cmd.find_first_not_of("MODEmode \t\v");
    if (found != std::string::npos)
        cmd = cmd.substr(found);
    else {
        _sendResponse(ERR_NEEDMOREPARAMS(cli->GetNickName(), "MODE"), fd);
        return;
    }

    getCmdArgs(cmd, channelName, modeset, params);
    std::vector<std::string> tokens = splitParams(params);

    if (channelName.empty() || channelName[0] != '#' || !(channel = GetChannel(channelName.substr(1)))) {
        _sendResponse(ERR_CHANNELNOTFOUND(cli->GetUserName(), channelName), fd);
        return;
    }

    if (!channel->get_client(fd) && !channel->get_admin(fd))
    {
        senderror(442, cli->GetNickName(), channelName, cli->GetFd(), " :You're not on that channel\r\n");
        return;
    }

    if (modeset.empty())
    {
        _sendResponse(
            RPL_CHANNELMODES(cli->GetNickName(), channel->GetName(), channel->getModes()) +
            RPL_CREATIONTIME(cli->GetNickName(), channel->GetName(), channel->get_creationtime()),
            fd);
        return;
    }

    if (!channel->get_admin(fd))
    {
        _sendResponse(ERR_NOTOPERATOR(channel->GetName()), fd);
        return;
    }

    size_t pos = 0;
    for (size_t i = 0; i < modeset.length(); ++i)
    {
        char mode_char = modeset[i];
        if (mode_char == '+' || mode_char == '-')
        {
            opera = mode_char;
        }
        else
        {
            if (mode_char == 'i')
                mode_chain << invite_only(channel, opera, mode_chain.str());
            else if (mode_char == 't')
                mode_chain << topic_restriction(channel, opera, mode_chain.str());
            else if (mode_char == 'k')
                mode_chain << password_mode(tokens, channel, pos, opera, fd, mode_chain.str(), arguments);
            else if (mode_char == 'o')
                mode_chain << operator_privilege(tokens, channel, pos, fd, opera, mode_chain.str(), arguments);
            else if (mode_char == 'l')
                mode_chain << channel_limit(tokens, channel, pos, opera, fd, mode_chain.str(), arguments);
            else
                _sendResponse(ERR_UNKNOWNMODE(cli->GetNickName(), channel->GetName(), std::string(1, mode_char)), fd);
        }
    }

    std::string chain = mode_chain.str();
    if (!chain.empty())
    {
        channel->sendTo_all(RPL_CHANGEMODE(cli->getHostname(), channel->GetName(), chain, arguments));
    }
}

std::string Server::invite_only(Channel* channel, char opera, const std::string& chain) {
    if (opera == '+' && !channel->getModeAtindex(0))
    {
        channel->setModeAtindex(0, true);
        channel->SetInvitOnly(1);
        return mode_toAppend(chain, opera, 'i');
    }
    else if (opera == '-' && channel->getModeAtindex(0))
    {
        channel->setModeAtindex(0, false);
        channel->SetInvitOnly(0);
        return mode_toAppend(chain, opera, 'i');
    }
    return "";
}

std::string Server::topic_restriction(Channel* channel, char opera, const std::string& chain) {
    if (opera == '+' && !channel->getModeAtindex(1))
    {
        channel->setModeAtindex(1, true);
        channel->set_topicRestriction(true);
        return mode_toAppend(chain, opera, 't');
    }
    else if (opera == '-' && channel->getModeAtindex(1))
    {
        channel->setModeAtindex(1, false);
        channel->set_topicRestriction(false);
        return mode_toAppend(chain, opera, 't');
    }
    return "";
}

bool validPassword(const std::string& password)
{
    if (password.empty())
        return false;

    for (size_t i = 0; i < password.length(); ++i)
    {
        char c = password[i];
        if (!std::isalnum(c) && c != '_')
            return false;
    }
    return true;
}

std::string Server::password_mode(const std::vector<std::string>& tokens, Channel* channel, size_t& pos, char opera, int fd, const std::string& chain, std::string& arguments) {
    std::string param, pass;
    if (tokens.size() > pos) {
        pass = tokens[pos++];
    } else {
        _sendResponse(ERR_NEEDMODEPARM(channel->GetName(), "(k)"), fd);
        return param;
    }

    if (!validPassword(pass))
    {
        _sendResponse(ERR_INVALIDMODEPARM(channel->GetName(), "(k)"), fd);
        return param;
    }

    if (opera == '+')
    {
        channel->setModeAtindex(2, true);
        channel->SetPassword(pass);
        if (!arguments.empty())
            arguments += " ";
        arguments += pass;
        return mode_toAppend(chain, opera, 'k');
    }
    else if (opera == '-' && channel->getModeAtindex(2))
    {
        if (pass == channel->GetPassword())
        {
            channel->setModeAtindex(2, false);
            channel->SetPassword("");
            return mode_toAppend(chain, opera, 'k');
        }
        else
        {
            _sendResponse(ERR_KEYSET(channel->GetName()), fd);
        }
    }
    return "";
}

std::string Server::operator_privilege(const std::vector<std::string>& tokens, Channel* channel, size_t& pos, int fd, char opera, const std::string& chain, std::string& arguments) {
    std::string param, user;
    if (tokens.size() > pos) {
        user = tokens[pos++];
    } else {
        _sendResponse(ERR_NEEDMODEPARM(channel->GetName(), "(o)"), fd);
        return param;
    }

    if (!channel->clientInChannel(user))
    {
        _sendResponse(ERR_NOSUCHNICK(channel->GetName(), user), fd);
        return param;
    }

    if (opera == '+')
    {
        channel->setModeAtindex(3, true);
        if (channel->change_clientToAdmin(user))
        {
            param = mode_toAppend(chain, opera, 'o');
            if (!arguments.empty())
                arguments += " ";
            arguments += user;
        }
    }
    else if (opera == '-')
    {
        channel->setModeAtindex(3, false);
        if (channel->change_adminToClient(user))
        {
            param = mode_toAppend(chain, opera, 'o');
            if (!arguments.empty())
                arguments += " ";
            arguments += user;
        }
    }
    return param;
}

bool Server::isvalid_limit(const std::string& limit) {
    return (limit.find_first_not_of("0123456789") == std::string::npos) && (std::atoi(limit.c_str()) > 0);
}

std::string Server::channel_limit(const std::vector<std::string>& tokens, Channel* channel, size_t& pos, char opera, int fd, const std::string& chain, std::string& arguments) {
    std::string param, limit;
    if (opera == '+')
    {
        if (tokens.size() > pos)
        {
            limit = tokens[pos++];
            if (!isvalid_limit(limit))
            {
                _sendResponse(ERR_INVALIDMODEPARM(channel->GetName(), "(l)"), fd);
            }
            else
            {
                channel->setModeAtindex(4, true);
                channel->SetLimit(std::atoi(limit.c_str()));
                if (!arguments.empty())
                    arguments += " ";
                arguments += limit;
                param = mode_toAppend(chain, opera, 'l');
            }
        }
        else
        {
            _sendResponse(ERR_NEEDMODEPARM(channel->GetName(), "(l)"), fd);
        }
    }
    else if (opera == '-' && channel->getModeAtindex(4))
    {
        channel->setModeAtindex(4, false);
        channel->SetLimit(0);
        param = mode_toAppend(chain, opera, 'l');
    }
    return param;
}
