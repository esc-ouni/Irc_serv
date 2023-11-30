

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

    std::map<int, Client> &get_clients()
    {
        return _clients;
    }

};
