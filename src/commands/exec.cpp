#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

void excute_command(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> &clients)
{
    if (command.substr(0, 5) == "PASS\n")
    { 
        std::string msg = ":localhost 431 ! :There is no password.\r\n";
        send(client.get_fd(), msg.c_str() , msg.length(), 0);
    }
    if (command.substr(0, 5) == "PASS ")
    { 
        if (pass(command, client) == false)
        {
            send(client.get_fd(), ERR(std::string(" wrong password")).c_str(), \
            ERR(std::string(" wrong password")).length(), 0);
        }
    }
    else if (command.substr(0, 4) == "NICK" && client.hasPassword() == false)
    {
        std::string msg = ":localhost 431 ! :There is no nickname | no password.\r\n";
        send(client.get_fd(), msg.c_str() , msg.length(), 0);
    }
    else if (command.substr(0, 5) == "NICK " && client.hasPassword() == true)
    {
        nick(command, client, clients, channels);
    }
    else if (command.substr(0, 5) == "USER\n" && client.is_authenticated() == true)
    {
        std::string msg = ":localhost 462 ! :You may not reregister.\r\n";
        send(client.get_fd(), msg.c_str(), msg.length(), 0);
    }
    else if (command.substr(0, 5) == "USER\n" && client.is_authenticated() == false)
    {
        send(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "USER").c_str(), \
        ERR_NEEDMOREPARAMS(client.get_nickname(), "USER").length(), 0);
    }
    else if (command.substr(0, 5) == "USER " && client.hasPassword() == true && client.hasNickname() == true)
    {
        user(command, client);
    }
    else if (command.substr(0, 5) == "JOIN " && client.is_authenticated()){
        handle_Join(command, client, channels);
    }
    else if (command.substr(0, 6) == "TOPIC " && client.is_authenticated()){
        set_topic(command, client, channels);
    }
    else if (command.substr(0, 5) == "PART " && client.is_authenticated()){
        leave_channel(command, client, channels);
    }
    else if (command.substr(0, 5) == "QUIT " && client.is_authenticated()){
        quit_server(client, channels);
    }
    else if (command.substr(0, 5) == "KICK " && client.is_authenticated()){
        kick_user(command, client, channels, clients);
    }
    else if (command.substr(0, 8) == "PRIVMSG " && client.is_authenticated())
    {
        privmsg(command, client, clients, channels);
    }
    else if (command.substr(0, 7) == "INVITE " && client.is_authenticated()){
        invite_user(command, client, channels, clients);
    }
    else if (command.substr(0, 4) == "MODE" && client.is_authenticated()){
        mode(command, client, channels, clients);
    }
    else if (command.substr(0, 5) == "OPER " && client.is_authenticated()){
        oper(command, client);
    }
    else if (command.substr(0, 11) == "STOPTALKING" && client.is_authenticated()){
        trigbot(command, client, clients);
    }
    // else if (command.substr(0, 5) == "AUTH ")
    // {
    //     std::string auth = ":localhost 001 " + client.get_nickname() + " :You are now " + (client.is_authenticated() ? "authenticated" : "not authenticated") + "\r\n";
    //     send(client.get_fd(), auth.c_str(), auth.length(), 0);
    // }
    // else if (command.substr(0, 8) == "DOWNLOAD" && client.is_authenticated())
    // {
    //     Irc::handleBot(client, command);
    // }
};