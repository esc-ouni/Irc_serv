

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
        void        addUser(Client &client);
        void        promote(Client &client);
        void        unpromote(Client &client); // need to be added when clients leave
        bool        is_operator(Client &client);
        void        set_topic(Client &client, std::string &new_topic);
        std::string get_topic() const;

};


bool        channel_name_is_valid(std::string &channel_name);
bool        valid_sp_character(int c);
bool        is_valid_topic(std::string &new_topic);
void        handleJoinCommand(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> &clients);
std::string Get_Users_list(std::map<int, Client> &clients, Channel &channel);
void        set_topic(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients);
