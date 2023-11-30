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
    bool _is_authenticated;
    bool _has_password;
    bool _has_nickname;
    unsigned long _start;

public:
    Client(int);

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
    bool hasPassword() const {
        return _has_password;
    }

    void setHasPassword(bool hasPassword) {
        _has_password = hasPassword;
    }

    bool hasNickname() const {
        return _has_nickname;
    }

    void setHasNickname(bool hasNickname) {
        _has_nickname = hasNickname;
    }

    std::string get_nickname()
    {
        return _nickname;
    }

    std::string get_username()
    {
        return _username;
    }

    int get_socket_fd()
    {
        return _fd;
    }

    void set_nickname(std::string nickname)
    {
        _nickname = nickname;
    }

    bool is_authenticated()
    {
        return _is_authenticated;
    }

    void set_authenticated(bool is_authenticated)
    {
        _is_authenticated = is_authenticated;
    }

    void set_username(std::string username)
    {
        _username = username;
    }

};
