#pragma once
// #include "Channel.hpp"
#include "Irc.hpp"

class Channel;

class Client
{
private:
    int              _fd;

    std::string      _buffer;
    std::string      _username;
    std::string      _nickname;
    std::string      _password;
    std::string      _servername;
    bool             _is_authenticated;
    bool             _has_password;
    bool             _has_nickname;
    unsigned long    _start;

public:
    Client();
    bool is_operator(Channel &channel);
    
    Client(int);
    std::string get_nickname();
    std::string get_username();
    std::string get_socket_fd_str();
    std::string get_servername();

    std::string get_topic(Channel &channel);
    bool        set_topic(Channel &channel, std::string &topic);

    
    int get_socket_fd();
    int leave_channel(Channel &channel);
    int kick_user(Client &, Channel &);
    bool is_authenticated();
    void set_authenticated(bool);
    void set_username(std::string);
    void set_nickname(std::string nickname);
    void set_servername(std::string servername);

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

    void add_buffer(std::string tmp)
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
    // Getter for _has_password
    bool hasPassword() const {
        return _has_password;
    }

    // Setter for _has_password
    void setHasPassword(bool hasPassword) {
        _has_password = hasPassword;
    }

    // Getter for _has_nickname
    bool hasNickname() const {
        return _has_nickname;
    }

    // Setter for _has_nickname
    void setHasNickname(bool hasNickname) {
        _has_nickname = hasNickname;
    }
};
