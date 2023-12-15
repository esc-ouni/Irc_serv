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
                send(it->second.get_fd(), RPL_PRIVMSG(client.get_nickname(), it->second.get_username(), receiver, to_send).c_str(), RPL_PRIVMSG(client.get_nickname(), it->second.get_username(), receiver, to_send).length(), 0);
                break;
            }
        }
        if (it == clients.end())
        {
            send(client.get_fd(), ERR_NOSUCHNICK(client.get_nickname(), receiver).c_str(), ERR_NOSUCHNICK(client.get_nickname(), receiver).length(), 0);
        }
    }
}

std::string remove_two_pts(std::string str)
{
    std::string result = str;

    result.erase(std::remove(result.begin(), result.end(), ':'), result.end());
    result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
    result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
    result.erase(std::remove(result.begin(), result.end(), '\t'), result.end());
    result.erase(std::remove(result.begin(), result.end(), '\v'), result.end());
    result.erase(std::remove(result.begin(), result.end(), '\f'), result.end());
    // std::cout << "result: '" << result << "'"<<  std::endl;
    return (result);
}

std::string remove_two_pts_and_space(std::string str) {
    std::string result = str;

    std::string::iterator it = result.begin();
    while (it != str.end() && (*it == ':' || *it == ' '))
        it++;
    result.erase(std::remove(result.begin(), it, ' '), it);
    result.erase(std::remove(result.begin(), it, ':'), it);
    result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
    result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
    result.erase(std::remove(result.begin(), result.end(), '\t'), result.end());
    result.erase(std::remove(result.begin(), result.end(), '\v'), result.end());
    result.erase(std::remove(result.begin(), result.end(), '\f'), result.end());
    // std::cout << "result: '" << result << "'"<<  std::endl;
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

    return (remove_two_pts_and_space(result));
}

std::string receivername(std::string message)
{
    std::istringstream stream(message);
    std::string receiver;

    stream >> receiver; 
    stream >> receiver; 

    return (receiver);
}

int chanel_name(const std::string receiver)
{
    if (!receiver.empty() && receiver[0] == '#')
        return 1;
    else
        return 0;
}

bool checkIfTheChennelExists(std::map<std::string, Channel> &channels,std::string channel)
{
    std::map<std::string, Channel>::iterator it;
    for (it = channels.begin() ; it != channels.end(); ++it)
    {
        if (it->second.get_name() == channel)
        {
            return (true);
        }    
    }
    return (false);
}

bool chechIfClientIsInChannel(std::map<std::string, Channel> &channels, Client &client, std::string channel)
{
    std::map<std::string, Channel>::iterator it;
    for (it = channels.begin() ; it != channels.end(); ++it)
    {
        if (it->second.get_name() == channel)
        {
            if (it->second.is_member(client))
            {
                return (true);
            }
        }    
    }
    return (false);
}

void broadcast_to_channel(std::string msg_to_send, Client &client, std::map<std::string, Channel> &channels,std::string channel)
{
    std::string cha = channel;

    if (!checkIfTheChennelExists(channels, cha))
    {
        ERR_NOSUCHCHANNEL(client.get_nickname(), cha);
        send(client.get_fd(), ERR_NOSUCHCHANNEL(client.get_nickname(), cha).c_str(), ERR_NOSUCHCHANNEL(client.get_nickname(), cha).length(), 0);
    }
    if (!chechIfClientIsInChannel(channels, client ,cha))
    {
        send(client.get_fd(), ERR_CANNOTSENDTOCHAN(client.get_nickname(), cha).c_str(), ERR_CANNOTSENDTOCHAN(client.get_nickname(), cha).length(), 0);
    }
    else
    {
        std::string sessage = msg_to_send + "\r\n";
        std::string Message = RPL_PRIVMSG(client.get_nickname(), client.get_nickname(), cha, remove_two_pts_and_space(sessage));

        channels[cha].broadcast_message_exp(client, Message);
    }

}

void privmsg(std::string message, Client &client, std::map<int, Client> &clients, std::map<std::string, Channel> &channels)
{
    std::string receiver = receivername(message);
    std::string msg_to_send = extract_msg_to_send(message);
    int chanel = chanel_name(receiver);
    

    if (receiver.empty())
        send(client.get_fd(), ERR_NORECIPIENT(client.get_nickname()).c_str(), ERR_NORECIPIENT(client.get_nickname()).length(), 0);
    else if (!receiver.empty() && chanel == 0)
    {
        privmsg_user(msg_to_send, client, clients, receiver);
    }
    else if (chanel == 1)
    {
        broadcast_to_channel(msg_to_send, client, channels, receiver);
    }
}
