

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


    std::map<int, Client> &get_clients()
    {
        return _clients;
    }


    std::string getName() const {
        return _name;
    }

    void setName(const std::string& name) {
        _name = name;
    }

    const std::map<int, Client>& getClients() const {
        return _clients;
    }

    void setClients(const std::map<int, Client>& clients) {
        _clients = clients;
    }

    std::string getTopic() const {
        return _topic;
    }

    void setTopic(const std::string& topic) {
        _topic = topic;
    }

    const std::map<char, char>& getModes() const {
        return _modes;
    }

    void setModes(const std::map<char, char>& modes) {
        _modes = modes;
    }

    const std::map<int, Client>& getOperators() const {
        return _operators;
    }

    void setOperators(const std::map<int, Client>& operators) {
        _operators = operators;
    }

    const std::vector<std::string>& getInvitees() const {
        return _invitees;
    }

    void setInvitees(const std::vector<std::string>& invitees) {
        _invitees = invitees;
    }

    std::string getKey() const {
        return _key;
    }

    void setKey(const std::string& key) {
        _key = key;
    }

    int getLimit() const {
        return _limit;
    }

    void setLimit(int limit) {
        _limit = limit;
    }

};

void handleJoinCommand(std::string command, Client &client, std::map<std::string, Channel> &channels);
