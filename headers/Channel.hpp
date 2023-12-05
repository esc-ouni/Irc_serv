

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
        std::string                 _topic_setter;
        std::string                 _topic_date;
        std::map<int, Client>       _clients;
        std::map<int, Client>       _operators;


        std::map<char, char>        _modes;
        std::vector<std::string>    _invitees;
        std::string                 _key;
        int                         _limit;
    public:
        void        add_user(Client &client);
        void        promote(Client &client);
        void        unpromote(Client &client); // need to be added when client leave
        bool        is_operator(Client &client);
        bool        set_topic(Client &client, std::string &new_topic);
        std::string get_topic() const;


        void        remove_user(Client &client);
        bool        is_member(Client &client);
        int         is_member(std::string &client_name);
        void        set_name(std::string &new_name);
        std::string get_name() const;
        std::string get_topic_setter() const;
        std::string get_topic_date() const;
        void        broadcast_message(std::string &message);
        void        broadcast_message_exp(Client &client, std::string &message);

        std::string get_all_users();

        

    std::map<int, Client> &get_clients()
    {
        return _clients;
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
