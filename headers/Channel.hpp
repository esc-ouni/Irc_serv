

#pragma once

#include "Irc.hpp"
// #include "Client.hpp"
#include <map>

typedef struct mode_options{
    bool         i;
    bool         t;
    bool         k;
    bool         l;
    unsigned int limit;
}   t_mode_options;


class Client;

class Channel
{
    private:
        std::string                   _name;
        std::string                   _topic;
        std::string                   _topic_setter;
        std::string                   _topic_date;
        std::string                   _creation_date;
        std::map<int, Client*>        _clients;   
        std::map<int, Client*>        _operators;
        std::map<int, Client*>        _invitees;
        t_mode_options                _modes;
        unsigned int                  _total_clients;
  
        std::string                   _password;
        bool                          _locked;

    public:
        Channel();
        ~Channel();
        bool        add_user(Client &client);
        void        promote(Client &client);
        void        unpromote(Client &client); // need to be added when client leave
        void        unpromote(int client_fd); // need to be added when client leave
        bool        set_topic(Client &client, std::string &new_topic);
        std::string get_topic() const;
        bool        is_operator(Client &client);
        bool        is_operator(std::string &client_name);


        void        remove_user(Client &client);
        bool        is_member(Client &client);
        int         is_member(std::string &client_name);
        void        set_name(std::string &new_name);
        std::string get_name() const;
        std::string get_topic_setter() const;
        std::string get_topic_date() const;
        void        broadcast_message(std::string message);
        void        broadcast_message_exp(Client &client, std::string message);
        std::string get_creation_date() const;
        std::string get_all_users();

        std::string get_password() const;
        bool        set_password(std::string password);

        int         get_limit() const;
        bool        set_limit(int limit);

        bool        get_option_l() const;
        bool        get_option_k() const;
        bool        get_option_t() const;
        bool        get_option_i() const;

        bool        set_option_l(bool bit);
        bool        set_option_k(bool bit);
        bool        set_option_t(bool bit);
        bool        set_option_i(bool bit);
        void        lock();
        void        unlock();
        std::string show_mode();    
        
        bool        add_to_invitee(Client &client);
        bool        is_invited(Client &client);
        void        remove_from_invite_list(Client &client);


        void        printChannelInfo();
};
