#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

bool hasInvalidChar(const std::string &name)
{
    std::string invalid = " $,*?:!#@.";

    for (size_t i = 0; i < name.size(); ++i)
    {
        for (size_t j = 0; j < invalid.size() - 1; ++j)
        {
            if (name[i] == invalid[j])
            {
                return (true);
            }
        }
    }
    return (false);
}

bool client_already_exist(std::string nickname, std::map<int, Client> clients)
{
    std::map<int, Client>::iterator it;

    for (it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->second.get_nickname() == nickname)
            return (true);
    }
    return (false);
}

std::string remove_two_pts(std::string str);


void nick(std::string command, Client &client, std::map<int, Client> clients, std::map<std::string, Channel> &channels)
{
    std::string nickname = filteredString(command.substr(5, command.length() - 5));

    if (client.hasNickname() == false)
    {
        if (remove_two_pts(nickname).empty())
        {    
            send(client.get_fd(), ERR_NONICKNAMEGIVEN , sizeof(ERR_NONICKNAMEGIVEN), 0);
            return;
        }

        else if (client_already_exist(nickname, clients))
        {
            send(client.get_fd(), ERR_NICKNAMEINUSE(client.get_nickname(), nickname).c_str(), ERR_NICKNAMEINUSE(client.get_nickname(), nickname).length(), 0);
            return;
        }
        else if (hasInvalidChar(nickname))
        {
            send(client.get_fd(), ERR_ERRONEUSNICKNAME(nickname).c_str(), ERR_ERRONEUSNICKNAME(nickname).length(), 0);
            return;
        }
        else
        {
            client.set_nickname(nickname);
            client.setHasNickname(true);
            return;
        }
    }
    // if authenticated client want to change his nickname
    else
    {
        if (remove_two_pts(nickname).empty())
        {
            send(client.get_fd(), ERR_NONICKNAMEGIVEN , sizeof(ERR_NONICKNAMEGIVEN), 0);
            return;
        }
        else if (client_already_exist(nickname, clients))
        {
            send(client.get_fd(), ERR_NICKNAMEINUSE(client.get_nickname(), nickname).c_str(), ERR_NICKNAMEINUSE(client.get_nickname(), nickname).length(), 0);
            return;
        }
        else if (hasInvalidChar(nickname))
        {
            send(client.get_fd(), ERR_ERRONEUSNICKNAME(nickname).c_str(), ERR_ERRONEUSNICKNAME(nickname).length(), 0);
            return;
        }
        else if (client.is_authenticated() == true)
        {
            std::string old_nick = client.get_nickname();
            client.set_nickname(nickname);
            
            //
            std::map<std::string, Channel>::iterator it = channels.begin();
            std::string reply = RPL_NICKCHANGED(old_nick, nickname);

            send_message(client.get_fd(), reply);
                while (it != channels.end()){
                if (channels[it->first].is_member(client)){
                    channels[it->first].broadcast_message_exp(client, reply);
                }
                it++;
            }
            //

            return;
        }
    }
}