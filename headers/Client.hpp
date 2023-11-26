#pragma once
#include "Irc.hpp"

class Channel;

class Client
{
private:
    int _fd;

    std::string _buffer;
    std::string _username;
    std::string _nickname;
    std::string _password;
    std::string _servername;
    std::string _buff_to_send;
    std::string _old_nick;
    bool _is_authenticated;
    unsigned long _start;

public:
    Client(int);
    std::string get_nickname();
    std::string get_username();
    std::string view_topic(Channel &);
    std::string get_socket_fd_str();
    std::string get_servername();
    std::string get_old_nick();
    std::string get_buff_to_send();
    int set_topic(Channel &, std::string);
    bool is_operator(Channel &);
    int get_socket_fd();
    int leave_channel(Channel &channel);
    int kick_user(Client &, Channel &);
    bool is_authenticated();
    void set_authenticated(bool);
    void set_username(std::string);
    void set_nickname(std::string nickname);
    void set_servername(std::string servername);
    void add_buffer_to_send(std::string);
    void set_old_nick(std::string);
    void set_buff_to_send(std::string);


    std::string get_pass()
    {
        return _password;
    };
    void set_pass(std::string pass)
    {
        _password = pass;
    };

    void set_buffer(std::string newValue)
    {
        _buffer = newValue;
    }

    std::string get_buffer()
    {
        return _buffer;
    }

    void addt_buffer(std::string tmp)
    {
        _buffer += tmp;
    }

    void set_fd(int newValue)
    {
        _fd = newValue;
    }

    int get_fd()
    {
        return _fd;
    }

    void setStart(unsigned long value)
    {
        _start = value;
    }

    unsigned long getStart()
    {
        return _start;
    }
};
