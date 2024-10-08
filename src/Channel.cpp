/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 10:20:22 by idouni            #+#    #+#             */
/*   Updated: 2023/12/18 09:51:57 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Irc.hpp"
#include "../headers/Channel.hpp"
#include "../headers/Client.hpp"
#include "../headers/commands.hpp"


std::string Channel::show_mode(){
    std::string mode;

    if (this->_modes.i)
        mode += 'i';
    if (this->_modes.t)
        mode += 't';
    if (this->_modes.k)
        mode += 'k';
    if (this->_modes.l)
        mode += 'l';
    if (!mode.empty())
        mode.insert(0, "+");
    return (mode);
};

Channel::Channel(){
    this->_creation_date = time_to_string(time_teller());
    this->_modes.limit = CHANNEL_LIMIT;
    this->_modes.i = false;
    this->_modes.k = false;
    this->_modes.t = false;
    this->_modes.l = false;
    this->_locked = false;
};

Channel::~Channel(){
};

void   Channel::lock(){
    this->_locked = true;
};
void   Channel::unlock(){
    this->_locked = false;
};

bool is_valid_password(std::string pass){
    if (pass.empty() || pass.length() > 20 || pass.length() < 8)
        return(false);
    for (size_t i = 0; i < pass.length(); i++){
        if ((!std::isalnum(pass.c_str()[i]) && !valid_sp_character(pass.c_str()[i])) || std::isspace(pass.c_str()[i]))
            return(false);
    }
    return (true);
}

std::string Channel::get_creation_date() const{
    return (this->_creation_date);
};

std::string Channel::get_password() const{
    return (this->_password);
};

bool        Channel::set_password(std::string password){
    if (is_valid_password(password)){
        if (this->_password != password){
            this->_password = password;
            return (true);
        }
    }
    return (false);
};


bool        Channel::get_option_l() const{
    return (this->_modes.l);
};

bool        Channel::get_option_k() const{
    return (this->_modes.k);
};

bool        Channel::get_option_t() const{
    return (this->_modes.t);
};

bool        Channel::get_option_i() const{
    return (this->_modes.i);
};


bool        Channel::set_option_l(bool bit){
    if (bit != this->_modes.l){
        this->_modes.l = bit;
        return (true);
    }
    return (false);
};

bool        Channel::set_option_k(bool bit){
    if (bit != this->_modes.k){
        this->_modes.k = bit;
        return (true);
    }
    return (false);
};

bool        Channel::set_option_t(bool bit){
    if (bit != this->_modes.t){
        this->_modes.t = bit;
        return (true);
    }
    return (false);
};

bool        Channel::set_option_i(bool bit){
    if (bit != this->_modes.i){
        this->_modes.i = bit;
        return (true);
    }
    return (false);
};

int Channel::get_limit() const{
    return (this->_modes.limit);
}

bool Channel::set_limit(unsigned int limit) {
    if (limit > 0 && limit <= CHANNEL_LIMIT){
        if (this->_modes.limit != limit){
            this->_modes.limit = limit;
            return (true);
        }
    }
    return (false);
}

void Channel::broadcast_message(std::string message){
    
    std::map<int, Client*>::iterator it = this->_clients.begin();

    while (it != this->_clients.end()){
        send_message(it->first, message);
        it++;
    }
};

std::string Channel::get_all_users(){
    std::string                          ALL_USERS = "";
    std::map<int, Client*>::iterator it = this->_clients.begin();
    
    while (it != this->_clients.end()) {
        if (is_operator(*(it->second)))
            ALL_USERS += "@" + it->second->get_nickname() + " ";
        else
            ALL_USERS += it->second->get_nickname() + " ";
        it++;
    }
    return ALL_USERS;
}

void Channel::broadcast_message_exp(Client &client, std::string message){
    
    std::map<int, Client*>::iterator it = this->_clients.begin();

    while (it != this->_clients.end()){
        if (it->first != client.get_fd())
            send_message(it->first, message);
        it++;
    }
};

std::string Channel::get_topic_setter() const{
    return (this->_topic_setter);
};

std::string Channel::get_topic_date() const{
    return (this->_topic_date);
};

time_t time_teller(){
    time_t      current_time = time(NULL);
    return (current_time);
}


void Channel::remove_user(Client &client) {
    if (this->is_member(client)){
        this->unpromote(client);
        this->remove_from_invite_list(client);
        this->_clients.erase(client.get_fd());
    }
};

bool Channel::add_user(Client &client) {
    if (this->get_option_i() && !this->is_invited(client)){
        send_message(client.get_fd(), ERR_INVITEONLYCHAN(client.get_nickname(), this->get_name()));
        return false;
    }
    if (this->_clients.size() >= this->_modes.limit){
        send_message(client.get_fd(), ERR_CHANNELISFULL(client.get_nickname(), this->get_name()));
        return false;
    }
    this->_clients[client.get_fd()] = &client;
    if (this->is_invited(client))
        this->remove_from_invite_list(client);
    return true;
};

std::string Channel::get_name() const{
    return (this->_name);
};

void Channel::set_name(std::string &new_name){ // IMADD
    this->_name = new_name;
};

int  Channel::is_member(std::string &client_name){
    std::map<int, Client*>::iterator it = _clients.begin();

    while (it != _clients.end()){
        if (it->second->get_nickname() == client_name){
            return (it->first);
        }
        it++;
    }
    return (0);
};

bool Channel::is_member(Client &client){
    std::map<int, Client*>::iterator it = _clients.begin();

    while (it != _clients.end()){
        if (it->first == client.get_fd()){
            return (true);
        }
        it++;
    }
    return (false);
};

bool Channel::is_operator(Client &client){
    std::map<int, Client*>::iterator it = _operators.begin();

    while (it != _operators.end()){
        if (it->first == client.get_fd()){
            return (true);
        }
        it++;
    }
    return (false);
};

bool Channel::is_operator(std::string &client_name){
    std::map<int, Client*>::iterator it = _operators.begin();

    while (it != _operators.end()){
        if (it->second->get_nickname() == client_name){
            return (true);
        }
        it++;
    }
    return (false);
};

void Channel::promote(Client &client){
    this->_operators[client.get_fd()] = &client;
};


void Channel::unpromote(Client &client){
    if (is_operator(client)){
        this->_operators.erase(client.get_fd());
    };
};

void Channel::unpromote(int client_fd){
    if (is_operator(*(_operators.at(client_fd)))){
        this->_operators.erase(client_fd);
    };
};

bool  Channel::set_topic(Client &client, std::string &new_topic){    
    if (this->_topic.compare(new_topic) && is_valid_topic(new_topic)){
        this->_topic        = new_topic;
        this->_topic_setter = client.get_nickname();
        this->_topic_date   = time_to_string(time_teller());
        return (true);
    }
    return (false);
};

std::string Channel::get_topic() const{
    return (this->_topic);
};

bool Channel::add_to_invitee(Client &client){
    if (!is_invited(client)){
        this->_invitees[client.get_fd()] = &client;
        return (true);
    }
    return (false);

};

bool  Channel::is_invited(Client &client){
     std::map<int, Client*>::iterator it = this->_invitees.begin();
     
    while (it != _invitees.end()){
        if (it->first == client.get_fd()){
            return (true);
        }
        it++;
    }
    return (false);
};

void   Channel::remove_from_invite_list(Client &client){
    if (is_invited(client))
        this->_invitees.erase(client.get_fd());
};




