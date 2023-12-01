#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

void sendMessage(int clientSocket, const std::string& message);

// splite username
bool splitusername(const std::string& input) {
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
    std::time_t currentTime;
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
            send(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "USER").c_str(), ERR_NEEDMOREPARAMS(client.get_nickname(), "USER").length(), 0);
        else if (splitusername(username) == false)
        {
            send(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "USER").c_str(), ERR_NEEDMOREPARAMS(client.get_nickname(), "USER").length(), 0);
            client.set_buffer("");
        }
        else
        {
            std::string address = "localhost";
            std::stringstream ss;
            ss << ":" << client.get_fd() << "!" << username << "@" << address;

            std::string result = ss.str();

            std::string datetime = getCurrentTime();
            client.set_username(username);
            client.set_authenticated(true);
            send(client.get_fd(), RPL_WELCOME(result, client.get_nickname()).c_str(), RPL_WELCOME(result, client.get_nickname()).length(), 0);
            send(client.get_fd(), RPL_YOURHOST(client.get_nickname(), "the best irc server").c_str(), RPL_YOURHOST(client.get_nickname(), "the best irc server").length(), 0);
            send(client.get_fd(), RPL_CREATED(client.get_nickname(), datetime).c_str(), RPL_CREATED(client.get_nickname(), datetime).length(), 0);
            
            result = "*************************************************************";
            sendMessage(client.get_fd(),RPL(result, client.get_nickname()));
            result = "*                                                                                          *";
            sendMessage(client.get_fd(),RPL(result, client.get_nickname()));
            result = "*                       Welcome to the best Irc server                     *";
            sendMessage(client.get_fd(),RPL(result, client.get_nickname()));
            result = "*                                                                                           *";
            sendMessage(client.get_fd(),RPL(result, client.get_nickname()));
            result = "*    Connect with the world. Chat, share, and have a great    *";
            sendMessage(client.get_fd(),RPL(result, client.get_nickname()));
            result = "*                                                                                          *";
            sendMessage(client.get_fd(),RPL(result, client.get_nickname()));
            result = "*             Enjoy your stay and happy chatting!                      *";
            sendMessage(client.get_fd(),RPL(result, client.get_nickname()));
            result = "*                                                                                          *";
            sendMessage(client.get_fd(),RPL(result, client.get_nickname()));
            result = "*************************************************************";
            sendMessage(client.get_fd(),RPL(result, client.get_nickname()));
            send(client.get_fd(), RPL_NICK(client.get_nickname(), client.get_username(), client.get_nickname()).c_str(), RPL_NICK(client.get_nickname(), client.get_username(), client.get_nickname()).length(), 0);


        }
    }
}



