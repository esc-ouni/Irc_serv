/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 17:27:26 by idouni            #+#    #+#             */
/*   Updated: 2023/12/17 17:28:25 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

int get_client_fd(std::string &client_name, std::map<int, Client> &clients){
    std::map<int, Client>::iterator it = clients.begin();

    while (it != clients.end()){
        if (client_name == it->second.get_nickname())
            return (it->second.get_fd());
        ++it;
    }
    return (0);
}


bool user_exist(std::string &client_name, std::map<int, Client> &clients){
    std::map<int, Client>::iterator it = clients.begin();

    while (it != clients.end()){
        if (client_name == it->second.get_nickname())
            return (true);
        ++it;
    }
    return (false);
}

void invite_user(std::string command, Client &excuter, std::map<std::string, Channel>& channels, std::map<int, Client> &clients){
    std::vector<std::string> args = parser(command, ' ');
    int argc = args.size();

    std::string user;
    std::string channel_name;

    if (argc == 3){
        user         = args.at(1);
        channel_name = args.at(2);
        if (!user_exist(user, clients)){
            send_message(excuter.get_fd(), ERR_NOSUCHNICK(excuter.get_nickname(), user));
            return ;
        }
        if (!channel_exist(channels, channel_name)){
                send_message(excuter.get_fd(), ERR_NOSUCHCHANNEL(excuter.get_nickname(), channel_name));
            return ;   
        }
        if (!excuter.is_operator(channels[channel_name])){
            send_message(excuter.get_fd(), ERR_CHANOPRIVSNEEDED(excuter.get_nickname(), channel_name));
            return ;
        }
        if (channels[channel_name].is_member(user)){
            send_message(excuter.get_fd(), ERR_USERONCHANNEL(excuter.get_nickname(), user, channel_name));
            return ;
        }
        if (channels[channel_name].add_to_invitee(clients[get_client_fd(user, clients)])){
            send_message(excuter.get_fd(), RPL_INVITESEND(excuter.get_nickname(), user, channel_name));
            send_message(get_client_fd(user, clients), RPL_INVITENOTIFY(excuter.get_nickname(), user, channel_name));
        };
    }
    else
        send_message(excuter.get_fd(), ERR_NEEDMOREPARAMS(excuter.get_nickname(), "INVITE"));
    return ;
}

