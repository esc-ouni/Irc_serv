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

void nick(std::map<std::string, Channel> &channels, std::string command, Client &client, std::map<int, Client> clients)
{
    std::string nickname = filteredString(command.substr(5, command.length() - 5));

    if (client.hasNickname() == false)
    {
        if (nickname.empty())
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
        if (client_already_exist(nickname, clients))
        {
            send(client.get_fd(), ERR_NICKNAMEINUSE(client.get_nickname(), nickname).c_str(), ERR_NICKNAMEINUSE(client.get_nickname(), nickname).length(), 0);
            return;
        }
        else if (nickname.empty())
        {
            send(client.get_fd(), ERR_NONICKNAMEGIVEN , sizeof(ERR_NONICKNAMEGIVEN), 0);
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
            
            
            // todo : change nickname in all channels
            
            
            send(client.get_fd(), RPL_NICK(old_nick, client.get_username(), nickname).c_str(), RPL_NICK(old_nick, client.get_username(), nickname).length(), 0);
            return;
        }
    }
}