/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trigbot.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 16:31:54 by idouni            #+#    #+#             */
/*   Updated: 2023/12/17 17:36:38 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

void trigbot(std::string command, Client &client, std::map<int, Client> &clients){
    std::vector<std::string> args = parser(command, ' ');
    std::string              msg;
    std::string              bot_name = "Emet";
    int                      argc = args.size();
    int                      bot_fd = 0;


    if (argc == 2){
        if (args.at(0) == "STOPTALKING"){
            if (client_already_exist(args.at(1), clients)){
                if (!(bot_fd = get_client_fd(bot_name, clients))){
                    msg = "Sorry, The Bot Isn't Available Right Now !";
                    send_message(client.get_fd(), RPL(client.get_nickname(), msg));
                    return ;                  
                }
                msg = "MSG_TO_SD " + args.at(1);
                send_message(bot_fd, msg);
                return ;
            }
            else{
                send_message(client.get_fd(), ERR_NOSUCHNICK(client.get_nickname(), args.at(1)));
                return ;
            }
        }
        send_message(client.get_fd(), RPL("STOPTALKING <noisy_client>", client.get_nickname()));
        return ;
    }
    else
        send_message(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "STOPTALKING"));
};

