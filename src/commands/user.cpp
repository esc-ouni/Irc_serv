/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 13:24:57 by idouni            #+#    #+#             */
/*   Updated: 2023/12/07 14:55:50 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

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
    static std::time_t currentTime;
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
            std::string address = "IRCHub";
            std::stringstream ss;
            ss << ":" << client.get_fd() << "!" << username << "@" << address;

            std::string result = ss.str();

            std::string datetime = getCurrentTime();
            client.set_username(username);
            client.set_authenticated(true);
            send(client.get_fd(), RPL_WELCOME(result, client.get_nickname()).c_str(), RPL_WELCOME(result, client.get_nickname()).length(), 0);
            send(client.get_fd(), RPL_YOURHOST(client.get_nickname(), "1337Hub").c_str(), RPL_YOURHOST(client.get_nickname(), "1337Hub").length(), 0);
            send(client.get_fd(), RPL_CREATED(client.get_nickname(), datetime).c_str(), RPL_CREATED(client.get_nickname(), datetime).length(), 0);
            
            // result = "*************************************************************";
            // send(client.get_fd(), RPL(result, client.get_nickname()).c_str(), RPL(result, client.get_nickname()).length(), 0);
            // send_message(client.get_fd(), result);
            // result = RPL('   $$\   $$$$$$\   $$$$$$\  $$$$$$$$\ $$\   $$\           $$\       ', client.get_nickname());
                                                
    
// result = RPL("  db d8888b. d8888b. d88888D db   db db    db d8888b. ",client.get_nickname());
            // send_message(client.get_fd(), result);
// result = RPL(" o88 VP  `8D VP  `8D VP  d8' 88   88 88    88 88  `8D ",client.get_nickname());
//             send_message(client.get_fd(), result);
// result = RPL("  88   oooY'   oooY'    d8'  88ooo88 88    88 88oooY' ",client.get_nickname());
//             send_message(client.get_fd(), result);
// result = RPL("  88   ~~~b.   ~~~b.   d8'   88~~~88 88    88 88~~~b. ",client.get_nickname());
//             send_message(client.get_fd(), result);
// result = RPL("  88 db   8D db   8D  d8'    88   88 88b  d88 88   8D ",client.get_nickname());
//             send_message(client.get_fd(), result);
// result = RPL("  VP Y8888P' Y8888P' d8'     YP   YP ~Y8888P' Y8888P' ",client.get_nickname());
//             send_message(client.get_fd(), result);
                                                     
                                                     

        result = "    ____________________  __     __  ";
        send_message(client.get_fd(), RPL(result, client.get_nickname()));
        result = "   <  /__  /__  /__  / / / /_ __/ /_ ";
        send_message(client.get_fd(), RPL(result, client.get_nickname()));
        result = "   / / /_ < /_ <  / / /_/ / / / / __ \\";
        send_message(client.get_fd(), RPL(result, client.get_nickname()));
        result = "  / /___/ /__/ / / / __  / /_/ / /_/ /";
        send_message(client.get_fd(), RPL(result, client.get_nickname()));
        result = " /_//___/____/ /_/_/ /_/\\___/____/ ";
        send_message(client.get_fd(), RPL(result, client.get_nickname()));


                                                                                      
            result = "***********************************************************";
            send_message(client.get_fd(), RPL(result, client.get_nickname()));
    

            result = "                          Welcome to 1337Hub                     ";
            send_message(client.get_fd(), RPL(result, client.get_nickname()));
            result = "                                                                                                  ";
            send_message(client.get_fd(), RPL(result, client.get_nickname()));
            result = " Connect with the world. Chat, share, and have a great DAY   ";
            send_message(client.get_fd(), RPL(result, client.get_nickname()));
            result = "                                                                                                 ";
            send_message(client.get_fd(), RPL(result, client.get_nickname()));
            result = "                Enjoy your stay and happy chatting!                      ";
            send_message(client.get_fd(), RPL(result, client.get_nickname()));
            result = "                                                                                                 ";
            send_message(client.get_fd(), RPL(result, client.get_nickname()));
            result = "***********************************************************";
            send_message(client.get_fd(), RPL(result, client.get_nickname()));


            // send(client.get_fd(), RPL(result, client.get_nickname()).c_str(), RPL(result, client.get_nickname()).length(), 0);
            // send(client.get_fd(), RPL_NICK(client.get_nickname(), client.get_username(), client.get_nickname()).c_str(), RPL_NICK(client.get_nickname(), client.get_username(), client.get_nickname()).length(), 0);
        }
    }
}
