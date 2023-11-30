#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

void privmsg_user(std::string to_send, Client &client, std::map<int, Client> &clients, std::string receiver)
{
    std::map<int, Client>::iterator it;

    if (to_send.empty())
    {
        send(client.get_fd(), ERR_NOTEXTTOSEND(client.get_nickname()).c_str(), ERR_NOTEXTTOSEND(client.get_nickname()).length(), 0);
    }
    else
    {
        for (it = clients.begin(); it != clients.end(); ++it)
        {
            if (it->second.get_nickname() == receiver)
            {
                send(it->second.get_fd(), RPL_PRIVMSG(client.get_nickname(), client.get_username(), receiver, to_send).c_str(), RPL_PRIVMSG(client.get_nickname(), client.get_username(), receiver, to_send).length(), 0);
                break;
            }
        }
        if (it == clients.end())
        {
            send(client.get_fd(), ERR_NOSUCHNICK(client.get_nickname(), receiver).c_str(), ERR_NOSUCHNICK(client.get_nickname(), receiver).length(), 0);
        }
    }
}

std::string remove_two_pts(std::string &str)
{
    std::string result = str;

    result.erase(std::remove(result.begin(), result.end(), ':'), result.end());
    result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
    result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
    result.erase(std::remove(result.begin(), result.end(), '\t'), result.end());
    result.erase(std::remove(result.begin(), result.end(), '\v'), result.end());
    result.erase(std::remove(result.begin(), result.end(), '\f'), result.end());
    return (result);
}

std::string extract_msg_to_send(std::string message)
{
    std::istringstream stream(message);
    std::string word;

    stream >> word; 
    stream >> word; 

    std::ostringstream remaining_string;
    remaining_string << stream.rdbuf();
    std::string result = remaining_string.str();
    // std::cout << "result'" << result << "'\n" ;

    return (remove_two_pts(result));
}

std::string receivername(std::string message)
{
    std::istringstream stream(message);
    std::string receiver;

    stream >> receiver; 
    stream >> receiver; 

    return (receiver);
}

void privmsg(std::string message, Client &client, std::map<int, Client> &clients, std::map<std::string, Channel> &channels)
{
    std::string receiver = receivername(message);
    std::string msg_to_send = extract_msg_to_send(message);

    if (receiver.empty())
        send(client.get_fd(), ERR_NORECIPIENT(client.get_nickname()).c_str(), ERR_NORECIPIENT(client.get_nickname()).length(), 0);
    else if (!receiver.empty())
    {
        privmsg_user(msg_to_send, client, clients, receiver);
    }
}
