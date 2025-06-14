#include "../inc/CommandHandler.hpp"

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

Commands    getCmd(const char buf[Server::MAX_BUF]) {
    std::string command(buf);
    if (command[0] == '/')
        command = command.substr(1);
    std::string command_prefix = command.substr(0, 4);
    std::transform(command_prefix.begin(), command_prefix.end(), command_prefix.begin(), ::toupper);

    if (command_prefix == "PASS") {
        return (PASS);
    } else if (command_prefix == "USER") {
        return (USER);
    } else if (command_prefix == "NICK") {
        return (NICK);
    } else if (command_prefix == "JOIN") {
        return (JOIN);
    } else if (command_prefix == "PART") {
        return (PART);
    } else if (command_prefix == "PRIVMSG") {
        return (PRIVMSG);
    }
    return (INVALID);
}

void    handleBuffer(Client& client, const char buf[512], const std::string& password, std::string& output) {
    if (client.state == UNAUTHENTICATED) {
        if (getCmd(buf) != PASS) {
            output = "Please enter the password\r\n";
            return ;
        }
        // Compare password with correct one here
        std::string command(buf);
        std::string providedPassword = command.substr(5);
        if (providedPassword == password) {
            client.state = AUTHENTICATED;
            output = "User authenticated\r\n";
        } else {
            output = "Incorrect password\r\n";
            return;
        }
    } else if (client.state == AUTHENTICATED) {
        // Check if user enters USER and NICK here
        if (getCmd(buf) == NICK) {
            std::string command(buf);
            client.nickname = command.substr(5);
            output = client.nickname + "\r\n";
        } else if (getCmd(buf) == USER) {
            std::string command(buf);
            client.username = command.substr(5);
            output = client.username + "\r\n";
        }
        if (!client.nickname.empty() && !client.username.empty()) {
            client.state = REGISTERED;
            output = "User registered\r\n";
        }
    } else if (client.state == REGISTERED) {
        // Handle normal commands here
        if (getCmd(buf) == PRIVMSG) {
            std::string command(buf);
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
