#include "CommandHandler.hpp"

// Commands getCmd(const char buf[MAX_BUF]) {
//     if (!buf || std::strlen(buf) == 0) {
//         return INVALID;
//     }

//     std::string command(buf);

//     if (!command.empty() && command[0] == '/') {
//         command.erase(0, 1);
//     }

//     // Convert commands to uppercase CHANGE LATER
//     std::transform(command.begin(), command.end(), command.begin(), ::toupper);

//     // Extract the command keyword (up to first space or end of string)
//     size_t spacePos = command.find(' ');
//     std::string command_keyword = command.substr(0, spacePos);

//     static const std::unordered_map<std::string, Commands> command_map = {
//         {"PASS", PASS},
//         {"USER", USER},
//         {"NICK", NICK},
//         {"JOIN", JOIN},
//         {"PART", PART},
//         {"PRIVMSG", PRIVMSG}
//     };

//     auto it = command_map.find(command_keyword);
//     return (it != command_map.end()) ? it->second : INVALID;
// }

Commands    getCmd(const std::string& buf) {
    std::string command = buf;
    if (command[0] == '/')
        command = command.substr(1);
    // std::string command_prefix = command.substr(0, 4);
    std::transform(command.begin(), command.end(), command.begin(), ::toupper);

    if (command == "PASS") {
        return (PASS);
    } else if (command == "USER") {
        return (USER);
    } else if (command == "NICK") {
        return (NICK);
    } else if (command == "JOIN") {
        return (JOIN);
    } else if (command == "PART") {
        return (PART);
    } else if (command == "PRIVMSG") {
        return (PRIVMSG);
    }
    return (INVALID);
}

std::vector<std::string> split(const std::string &txt, std::string ch)
{
    std::vector<std::string> strs;
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return (strs);
}

Numeric parseBuffer(const char buf[], std::vector<std::string> &tokens) {
    tokens.clear();
    tokens = split(std::string(buf), " ");
    if (getCmd(tokens[0]) == PASS) {
        if (tokens.size() < 2) {
            return (ERR_NEEDMOREPARAMS);
        }
    }
    else if (getCmd(tokens[0]) == USER) {
        if (tokens.size() < 5) {
            return (ERR_NEEDMOREPARAMS);
        }
    }
    return (SUCCESS);
}

void    executeBuffer(std::vector<std::string> &tokens, Client &client, std::string &output) {
    if (client.state == UNAUTHENTICATED) {
        if (getCmd(tokens[0]) != PASS) {
            output = "Please enter the password\r\n";
            return ;
        }
        // if (providedPassword == password) {
            client.state = AUTHENTICATED;
            output = "User authenticated\r\n";
        // } else {
        //     output = "Incorrect password\r\n";
        //     return;
        // }
    } else if (client.state == AUTHENTICATED) {
        // Check if user enters USER and NICK here
        if (getCmd(tokens[0]) == NICK) {
            client.nickname = tokens[1];
            output = client.nickname + "\r\n";
        } else if (getCmd(tokens[0]) == USER) {
            client.username = tokens[1];
            client.realname = tokens[4];
            output = client.username + "\r\n";
        }
        if (!client.nickname.empty() && !client.username.empty()) {
            client.state = REGISTERED;
            output = "User registered\r\n";
        }
    } else if (client.state == REGISTERED) {
        // Handle normal commands here
        if (getCmd(tokens[0]) == PRIVMSG) {
            std::string command = tokens[0];
            size_t pos = command.find(":");
            if (pos != std::string::npos) {
                std::string target = command.substr(8, pos - 8 - 1);
                std::string message = command.substr(pos + 1);
                // TODO: Send message to target
                output = "Sending message to " + target + ": " + message + "\r\n";
            } else {
                output = "Invalid PRIVMSG format\r\n";
            }
        }
    }
}

void    handleBuffer(Client& client, const char buf[], const std::string& password, std::string& output) {
    (void) password;
    std::vector<std::string> tokens;
    if (parseBuffer(buf, tokens) == SUCCESS) {
        executeBuffer(tokens, client, output);
    }
}
