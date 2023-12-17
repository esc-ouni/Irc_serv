/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 13:40:06 by idouni            #+#    #+#             */
/*   Updated: 2023/12/17 13:40:07 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
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
    std::string      _client_host;


    bool             _is_authenticated;
    bool             _has_password;
    bool             _has_nickname;
    bool             _IRC_op;
    unsigned long    _start;

public:
    Client();
    Client(int);
    std::string get_nickname();
    std::string get_username();
    std::string get_socket_fd_str();
    std::string get_servername();

    bool        is_operator(Channel &channel);
    bool        set_topic(Channel &channel, std::string &topic);
    std::string get_topic(Channel &channel);
    void        promote_IRC_op();
    bool        is_IRC_op();
    std::string get_client_host() const;
    void        set_client_host(std::string client_host);
    
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
