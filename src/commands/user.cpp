#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

bool splitusername(const std::string& input)
{
    std::istringstream stream(input);
    std::string token;

    int count = 0;
    while (getline(stream, token, '0'))
    {
        count++;
        if (!token.empty())
        {
            if (count == 2)
            {
                count = 0;
                std::istringstream stream1(token);
                std::string token1;

                while (getline(stream1, token1, '*'))
                {
                    count++;
                    if (count == 2)
                      {
                        if (!token1.empty())
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                      }
                }
            }
        }
        else
        {
            return false;
        }
    }
    return false;
}

std::string getCurrentTime()
{
    long currentTime;
    std::time(&currentTime);

    struct tm* timeInfo = std::localtime(&currentTime);

    int hours = timeInfo->tm_hour;
    int minutes = timeInfo->tm_min;
    int seconds = timeInfo->tm_sec;

    std::stringstream ss;
    ss << hours << ":" << minutes << ":" << seconds;
    return (ss.str());
}

void user(std::string command, Client &client)
{
    std::string username = command.substr(5, command.length() - 5);

    if (client.is_authenticated() == false)
    {
        if (username.empty())
            send(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "USER").c_str(), \
            ERR_NEEDMOREPARAMS(client.get_nickname(), "USER").length(), 0);
        else if (splitusername(username) == false)
        {
            send(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "USER").c_str(), \
            ERR_NEEDMOREPARAMS(client.get_nickname(), "USER").length(), 0);
            client.set_buffer("");
        }
        else
        {
            std::string address = "localhost";
            std::stringstream ss;
            ss << ":" << client.get_fd() << "!" << username << "@" << address;

            std::string result = ss.str();

            std::string datetime = getCurrentTime();
            std::string user = filteredString(username);
            client.set_username(user);
            client.set_authenticated(true);
            send(client.get_fd(), RPL_WELCOME(result, client.get_nickname()).c_str(), \
            RPL_WELCOME(result, client.get_nickname()).length(), 0);
            send(client.get_fd(), RPL_YOURHOST(client.get_nickname(), "the best irc server").c_str(), \
            RPL_YOURHOST(client.get_nickname(), "the best irc server").length(), 0);
            send(client.get_fd(), RPL_CREATED(client.get_nickname(), datetime).c_str(), \
            RPL_CREATED(client.get_nickname(), datetime).length(), 0);
            send(client.get_fd(), RPL_MYINFO(client.get_nickname(), \
            "the best irc server", "version 2.0", "user_modes", "chan_modes", "chan_param_modes").c_str(), \
            RPL_MYINFO(client.get_nickname(), "the best irc server", "version 2.0", \
            "user_modes", "chan_modes", "chan_param_modes").length(), 0);
            send(client.get_fd(), RPL_ISUPPORT(client.get_nickname(), \
            "PASS NICK USER PRIVMSG DOWNLOAD JOIN KICK INVITE TOPIC MODE (+-itkol)").c_str(),\
             RPL_ISUPPORT(client.get_nickname(), "PASS NICK USER PRIVMSG DOWNLOAD JOIN KICK INVITE TOPIC MODE (+-itkol)").length(), 0);
            
            result = "*************************************************************";
            send(client.get_fd(), RPL(result, client.get_nickname()).c_str(), RPL(result, client.get_nickname()).length(), 0);
            result = "                                                                                                 ";
            send(client.get_fd(), RPL(result, client.get_nickname()).c_str(), RPL(result, client.get_nickname()).length(), 0);
            result = "                              Welcome to the best Irc server                     ";
            send(client.get_fd(), RPL(result, client.get_nickname()).c_str(), RPL(result, client.get_nickname()).length(), 0);
            result = "                                                                                                  ";
            send(client.get_fd(), RPL(result, client.get_nickname()).c_str(), RPL(result, client.get_nickname()).length(), 0);
            result = "           Connect with the world. Chat, share, and have a great    ";
            send(client.get_fd(), RPL(result, client.get_nickname()).c_str(), RPL(result, client.get_nickname()).length(), 0);
            result = "                                                                                                 ";
            send(client.get_fd(), RPL(result, client.get_nickname()).c_str(), RPL(result, client.get_nickname()).length(), 0);
            result = "                    Enjoy your stay and happy chatting!                      ";
            send(client.get_fd(), RPL(result, client.get_nickname()).c_str(), RPL(result, client.get_nickname()).length(), 0);
            result = "                                                                                                 ";
            send(client.get_fd(), RPL(result, client.get_nickname()).c_str(), RPL(result, client.get_nickname()).length(), 0);
            result = "*************************************************************";
            send(client.get_fd(), RPL(result, client.get_nickname()).c_str(), RPL(result, client.get_nickname()).length(), 0);
        }
    }
    else if (client.is_authenticated() == true)
    {
        send(client.get_fd(), ERR_ALREADYREGISTERED(client.get_nickname()).c_str(), ERR_ALREADYREGISTERED(client.get_nickname()).length(), 0);
    }
}
