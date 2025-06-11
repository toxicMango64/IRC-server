#include "../inc/CommandHandler.hpp"

Commands    getCmd(const char buf[512]) {
    const std::string command(buf);
    const std::string command_prefix = command.substr(0, 4);

    if (command_prefix == "pass") {
        return (PASS);
    } else if (command_prefix == "user") {
        return (USER);
    } else if (command_prefix == "nick") {
        return (NICK);
    } else if (command_prefix == "join") {
        return (JOIN);
    } else if (command_prefix == "part") {
        return (PART);
    }
    return (INVALID);
}

void    handleBuffer(Client& client, const char buf[512], std::string& output) {
    if (client.state == UNAUTHENTICATED) {
        if (getCmd(buf) != PASS) {
            output = "Please enter the password\r\n";
            return ;
        }
        // Compare password with correct one here
        client.state = AUTHENTICATED;
        output = "User authenticated\r\n";
    } else if (client.state == AUTHENTICATED) {
        // Check if user enters USER and NICK here
        if (getCmd(buf) == NICK) {
            client.nickname = std::string(buf).substr(5);
            output = client.nickname;
        }
        if (getCmd(buf) == USER) {
            client.username = std::string(buf).substr(5);
            output = client.username;
        }
        if (!client.nickname.empty() && !client.username.empty()) {
            client.state = REGISTERED;
            output = "User registered\r\n";
        }
    } else if (client.state == REGISTERED) {
        // Handle normal commands here
    }
}
