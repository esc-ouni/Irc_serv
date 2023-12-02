#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"


void excute_command(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> &clients)
{
    if (command.substr(0, 4) == "PASS")
    {
        if (pass(command, client) == false)
        {
            send(client.get_fd(), ERR(std::string(" wrong password")).c_str(), ERR(std::string(" wrong password")).length(), 0);
        }
    }
    else if (command.substr(0, 4) == "NICK" && client.hasPassword() == true)
    {
        nick(command, client, clients);
    }
    else if (command.substr(0, 4) == "USER" && client.hasPassword() == true && client.hasNickname() == true)
    {
        user(command, client);
    }
    else if (command.substr(0, 4) == "AUTH")
    {
        std::string auth = ":localhost 001 " + client.get_nickname() + " :You are now " + (client.is_authenticated() ? "authenticated" : "not authenticated") + "\r\n";
        send(client.get_fd(), auth.c_str(), auth.length(), 0);
    }
    else if (command.substr(0, 4) == "JOIN"){
        handleJoinCommand(command, client, channels, clients);
    }
    else if (command.substr(0, 5) == "TOPIC" && client.is_authenticated())
    {
        set_topic(command, client, channels, clients);
    }
};

//     else if (command.substr(0, 4) == "KICK" && client.is_authenticated())
//     {
//         kick(command, client, channels);
//     }
//     // INVITE
//     else if (command.substr(0, 6) == "INVITE" && client.is_authenticated())
//     {
//         invite(command, client, channels, clients);
//     }
//     else if (command.substr(0, 4) == "MODE" && client.is_authenticated())
//     {
//         mode(command, client, channels);
//     }
    // else if (command.substr(0, 7) == "PRIVMSG" && client.is_authenticated())
    // {
    //     privmsg(command, client, clients, channels);
    //     std::cout << "PRIVMSG" << std::endl;
    // }
//     else if (filteredString(command) == "LOGTIME" && client.is_authenticated())
//     {
//         Irc::handleLogTime(client);
//     }
//     else if (filteredString(command) == "QUOTES" && client.is_authenticated())
//     {
//         Irc::handleQuotes(client);
//     }
//     else if (filteredString(command) == "DOWNLOAD" && client.is_authenticated())
//     {
//         Irc::handleBot(client);
//     }

//     else if (filteredString(command.substr(0, 4))!= "QUIT")
//     {
        
//         send(client.get_fd(), ERR_UNKNOWNCOMMAND(command.substr(0, command.find(" "))).c_str(), ERR_UNKNOWNCOMMAND(command.substr(0, command.find(" "))).length(), 0);
//     }


//     if (client.is_authenticated() && !client.get_buff_to_send().empty())
//     {
//         send(client.get_fd(), client.get_buff_to_send().c_str(), client.get_buff_to_send().length(), 0);
//         client.set_buff_to_send("");
//     }