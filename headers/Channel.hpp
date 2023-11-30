

#pragma once

#include "Irc.hpp"
#include "Client.hpp"
#include <map>

class Client;

class Channel
{
private:
    std::string _name;
    std::map<int, Client> _clients;
    std::string _topic;
    std::map<char, char> _modes;
    std::map<int, Client> _operators;
    std::vector<std::string> _invitees;
    std::string _key;
    int _limit;
public:
    void addUser(Client client) {
        this->_clients[client.get_fd()] = client;
    };


};

void handleJoinCommand(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> &clients);
std::string Get_Users_list(std::map<int, Client> &clients);