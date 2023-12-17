/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 17:30:14 by idouni            #+#    #+#             */
/*   Updated: 2023/12/17 17:31:34 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

void kick_user(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients){
    std::vector<std::string> args = parser(command, ' ');
    int argc = args.size();

    std::string              user;
    std::string              channel_name;
    int                      target_fd = 0;
    std::string              reason;
    if (args.at(0) != "KICK")
        return ;
    if (argc >= 3){
        channel_name = args.at(1);
        user         = args.at(2);
        reason       = extract_reason(command);

        if(!channel_exist(channels, channel_name)){
            send_message(client.get_fd(), ERR_NOSUCHCHANNEL(client.get_nickname(), channel_name));
            return ; 
        }
        if (!channels[channel_name].is_operator(client)){
            send_message(client.get_fd(), ERR_CHANOPRIVSNEEDED(client.get_nickname(), channel_name));
            return ; 
        }
        target_fd = channels[channel_name].is_member(user);
        if (!target_fd){
            send_message(client.get_fd(), ERR_USERNOTINCHANNEL(client.get_nickname(), user, channel_name));
            return ; 
        }

        reason = RPL_KICK(client.get_nickname(), client.get_client_host(), channel_name, clients[target_fd].get_nickname() , reason);
        channels[channel_name].broadcast_message(reason);            
        channels[channel_name].remove_user(clients[target_fd]);
    }
    else
        send_message(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "KICK"));
};

