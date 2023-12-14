/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 16:31:54 by idouni            #+#    #+#             */
/*   Updated: 2023/12/14 16:52:55 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

void bot_call(std::string command, Client &client, std::__1::map<int, Client> &clients){
    std::vector<std::string> args = parser(command, ' ');
    std::string msg;
    std::string bot_name = "MusicBot";

    int bot_fd;

    int argc = args.size();
                
    std::cout << "DEBIG::TRIGGER_BOT" << std::endl;

    if (argc == 2){
        if (args[0] == "STOPTALKING"){
            if (client_already_exist(args[1], clients)){
                if (!(bot_fd = get_client_fd(bot_name, clients))){
                    msg = "Sorry, The Bot Isn't Available Right Now !";
                    send_message(client.get_fd(), RPL(client.get_nickname(), msg));
                    return ;                  
                }
                
                msg = "MSG_TO_SD " + args[1];
                send_message(bot_fd, msg);
                // send_message(client.get_fd(), RPL("Now Playing sba7_lkheir ...", client.get_nickname()));
                return ;
            }
            else{
                send_message(client.get_fd(), ERR_NOSUCHNICK(client.get_nickname(), args[1]));
                return ;
            }
        }
        send_message(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "STOPTALKING"));
        return ;
    }
};

