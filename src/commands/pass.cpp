#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

std::string filteredString(std::string str)
{
    std::string filteredstr;
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] != '\n' && str[i] != '\r' \
           && str[i] != '\t' && str[i] != '\v'\
           && str[i] != '\f' && str[i] != ' ')
        {
            filteredstr += str[i];
        }
    }
    return filteredstr;
}

bool pass( std::string command, Client &client)
{
    std::string pass = filteredString(command.substr(5, command.length() - 5));

    if (client.get_pass() != pass)
    {
        client.setHasPassword(false);
        return false;
    }
    else if (client.get_pass() == pass && client.is_authenticated() == true)
    {
        send(client.get_fd(), ERR_ALREADYREGISTERED(client.get_nickname()).c_str(), ERR_ALREADYREGISTERED(client.get_nickname()).length(), 0);
        return true;
    }
    else
    {
        client.setHasPassword(true);
        return true;
    } 
}