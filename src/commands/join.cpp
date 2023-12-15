/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 10:55:41 by idouni            #+#    #+#             */
/*   Updated: 2023/12/15 16:18:57 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

bool Create_channel_join(Client &client, std::map<std::string, Channel>& channels, std::string& new_channel_name) {
    if (!channels[new_channel_name].add_user(client))
        return (false);
    channels[new_channel_name].set_name(new_channel_name);
    channels[new_channel_name].promote(client);
    return (true);
};

bool channel_join(Client &client, std::map<std::string, Channel>& channels, std::string& channel_name, std::string& password){
    if (channels[channel_name].get_option_k()){
        if (channels[channel_name].get_password() != password){
            send_message(client.get_fd(), ERR_INVALIDKEY(client.get_nickname(), channel_name));
            return (false);
        }
    }
    if (channels[channel_name].is_member(client)){
        send_message(client.get_fd(), ERR_USERONCHANNEL(client.get_nickname(), client.get_nickname(), channel_name));
        return (false); 
    }
    if (!channels[channel_name].add_user(client)){
        return (false);
    }
    return (true);
};

void send_names_list(Client &client, Channel &channel){
    send_message(client.get_fd(), RPL_NAMREPLY(client.get_nickname(), channel.get_name(), channel.get_all_users()));
    send_message(client.get_fd(), RPL_ENDOFNAMES(client.get_nickname(), channel.get_name()));
}


void handle_Join(std::string command, Client &client, std::map<std::string, Channel>& channels) {
    std::vector<std::string> args = parser(command, ' ');
    int argc = args.size();

    std::vector<std::string> channel_list;
    std::string              password = "";
    std::string              reply = "";

    if (argc == 3 || argc == 2){
        if (argc == 3)
            password = args[2];
        channel_list = parser(args[1], ',');
        for (size_t i = 0; i < channel_list.size(); i++){
            if (!channel_list.at(i).empty() && channel_name_is_valid(channel_list.at(i))){
                if (!channel_exist(channels, channel_list.at(i))){
                    if(!Create_channel_join(client, channels, channel_list.at(i)))
                        return ;
                }
                else if (channel_exist(channels, channel_list.at(i))){
                    if(!channel_join(client, channels, channel_list.at(i), password)){
                        return ;
                    }
                }
                reply = RPL_NOTIFYJOIN(client.get_nickname(), client.get_client_host(), channel_list.at(i));
                channels[channel_list.at(i)].broadcast_message(reply);
                if (!channels[channel_list.at(i)].get_topic_date().empty()){
                    send_message(client.get_fd(), RPL_TOPIC(client.get_nickname(), channel_list.at(i), channels[channel_list.at(i)].get_topic()));
                    send_message(client.get_fd(), RPL_TOPICWHOTIME(client.get_nickname(), channel_list.at(i) , channels[channel_list.at(i)].get_topic_setter(), channels[channel_list.at(i)].get_topic_date()));
                }
                else
                    send_message(client.get_fd(), RPL_NOTOPIC(client.get_nickname(), channel_list.at(i)));
                send_names_list(client, channels[channel_list.at(i)]);
            }
        }
    }
    else
        send_message(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "JOIN"));
    return ;  
};
