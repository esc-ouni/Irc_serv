#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

bool is_multipe_words(std::string message)
{
    for (int i = 0; i < (int)message.length(); i++)
    {
        if (message[i] == ' ')
            return true;
    }
    return false;
}

std::string extractName(std::string message, int *spaces)
{
    std::string name;
    std::string::iterator it;
    // skip all spaces
    for (it = message.begin(); it != message.end(); ++it)
    {
        if (*it != ' ')
        {
            break;
        }
        *spaces += 1;
    }

    for (it = message.begin(); it != message.end(); ++it)
    {
        if (*it == ' ')
        {
            break;
        }
        name += *it;
    }
    return name;
}

void privmsg_user(std::string to_send, Client &client, std::map<int, Client> &clients, std::string receiver)
{
    std::map<int, Client>::iterator it2;
    if (to_send.empty())
        send(client.get_fd(), ERR_NOTEXTTOSEND(client.get_nickname()).c_str(), ERR_NOTEXTTOSEND(client.get_nickname()).length(), 0);
    else
    {
        for (it2 = clients.begin(); it2 != clients.end(); ++it2)
        {
            if (it2->second.get_nickname() == receiver)
            {
                send(it2->second.get_fd(), RPL_PRIVMSG(client.get_nickname(), client.get_username(), receiver, to_send).c_str(), RPL_PRIVMSG(client.get_nickname(), client.get_username(), receiver, to_send).length(), 0);
                break;
            }
        }
        if (it2 == clients.end())
        {
            send(client.get_fd(), ERR_NOSUCHNICK(client.get_nickname(), receiver).c_str(), ERR_NOSUCHNICK(client.get_nickname(), receiver).length(), 0);
        }
    }
}

void privmsg(std::string message, Client &client, std::map<int, Client> &clients, std::map<std::string, Channel> &channels)
{
    int spaces = 0;
    std::string receiver = extractName(message.substr(8, message.length() - 8), &spaces);
    std::string to_send = message.substr(8 + receiver.length() + 1 + spaces, message.length());


    to_send = (is_multipe_words(to_send) ? to_send.substr(1, to_send.length() - 1) : to_send);

    if (!receiver.empty())
    {
        privmsg_user(to_send, client, clients, receiver);
    }
}