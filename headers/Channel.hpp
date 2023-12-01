

#pragma once

#include "Irc.hpp"
// #include "Client.hpp"
#include <map>

class Client;

class Channel
{
    private:
        std::string                 _name;
        std::string                 _topic;
        std::map<int, Client>       _clients;
        std::map<int, Client>       _operators;


        std::map<char, char>        _modes;
        std::vector<std::string>    _invitees;
        std::string                 _key;
        int                         _limit;
    public:
        void addUser(Client &client);
        // void promote(Client &client);
        // void unpromote(Client &client){
        //     if (is_operator(client))

        // };
        bool is_operator(Client &client);

};

void handleJoinCommand(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> &clients);
std::string Get_Users_list(std::map<int, Client> &clients, Channel &channel);