#include "../headers/Irc.hpp"
#include "../headers/Channel.hpp"
#include "../headers/Client.hpp"

Client::Client(void){
    _fd = 0;
    _is_authenticated = false;

    _nickname = "";
    _username = "";
    _buffer = "";
    _IRC_op = false;

    struct timeval time;
    gettimeofday(&time, NULL);
    _start = time.tv_sec;
};

Client::Client(int fd)
{
    _fd = fd;
    _is_authenticated = false;

    _nickname = "";
    _username = "";
    _buffer = "";
    _IRC_op = false;

    struct timeval time;
    gettimeofday(&time, NULL);
    _start = time.tv_sec;
}

bool Client::is_operator(Channel &channel){
    return (channel.is_operator((*this)));

};

std::string Client::get_topic(Channel &channel){
    return (channel.get_topic());
};

bool   Client::set_topic(Channel &channel, std::string &topic){
    return (channel.set_topic((*this), topic));
};

void    Client::promote_IRC_op(){
    this->_IRC_op = true;
};

bool    Client::is_IRC_op(){
    return (this->_IRC_op);
};

std::string  Client::get_client_host() const{
    return (this->_client_host);
};

void  Client::set_client_host(std::string client_host){
    this->_client_host = client_host;
};



std::string Client::get_nickname()
{
    return _nickname;
}

std::string Client::get_username()
{
    return _username;
}

int Client::get_socket_fd()
{
    return _fd;
}

void Client::set_nickname(std::string nickname)
{
    _nickname = nickname;
}

bool Client::is_authenticated()
{
    return _is_authenticated;
}

void Client::set_authenticated(bool is_authenticated)
{
    _is_authenticated = is_authenticated;
}

void Client::set_username(std::string username)
{
    _username = username;
}

// int Client::set_operator(Channel &channel, Client &client)
// {
//     if (channel.get_clients().find(client.get_socket_fd()) == channel.get_clients().end())
//         return (-1);
//     channel.get_operators().insert(std::pair<int, Client>(client.get_socket_fd(), client));
//     return (0);
// }

// int Client::remove_operator(Channel &channel, Client &client)
// {
//     if (channel.get_clients().find(client.get_socket_fd()) == channel.get_clients().end())
//         return (-1);
//     channel.get_operators().erase(client.get_socket_fd());
//     return (0);
// }
