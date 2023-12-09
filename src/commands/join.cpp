/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 10:55:41 by idouni            #+#    #+#             */
/*   Updated: 2023/12/09 11:20:17 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

bool Create_channel_join(Client &client, std::map<std::string, Channel>& channels, std::string& new_channel_name, std::map<int, Client> &clients) {
    if (!channels[new_channel_name].add_user(client))
        return (false);
    channels[new_channel_name].set_name(new_channel_name);
    
    // promotes the user to be an operator
    channels[new_channel_name].promote(client);
    return (true);
};

bool channel_join(Client &client, std::map<std::string, Channel>& channels, std::string& channel_name, std::map<int, Client> &clients, std::string& password){
    if (channels[channel_name].get_option_k()){
        if (channels[channel_name].get_password() != password){
            send_message(client.get_fd(), ERR_PASSWDMISMATCH(client.get_nickname()));
            std::cout << "WORNG_PASS" << std::endl;
            return (false);
        }
    }
    if (!channels[channel_name].add_user(client)){
        send_message(client.get_fd(), ERR_CHANNELISFULL(client.get_nickname(), channel_name));
        std::cout << "ERR_CHANNELISFULL" << std::endl;
        return (false);
    }
    return (true);
};

void send_names_list(Client &client, Channel &channel){
    send_message(client.get_fd(), RPL_NAMREPLY(client.get_nickname(), channel.get_name(), channel.get_all_users()));
    send_message(client.get_fd(), RPL_ENDOFNAMES(client.get_nickname(), channel.get_name()));
}


void handle_Join(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients) {
    std::vector<std::string> args = parser(command, ' ');
    int argc = args.size();

    std::string         channel_list;
    std::string         channel_name;
    std::string         password = "";
    std::string         reply = "";

if (argc == 3 || argc == 2){
    if (argc == 3)
        password = args[2];
    channel_list = args[1];
    std::istringstream  stream(channel_list);

    while (std::getline(stream, channel_name, ',')) {
        if (!channel_name.empty() && channel_name_is_valid(channel_name)){
            if (!channel_exist(channels, channel_name)){
                if(!Create_channel_join(client, channels, channel_name, clients))
                    return ;
            }
            else if (channel_exist(channels, channel_name)){
                if(!channel_join(client, channels, channel_name, clients, password)){
                    return ;
                }
            }
            reply = RPL_NOTIFYJOIN(client.get_nickname(), channel_name);
            channels[channel_name].broadcast_message(reply);
            if (!channels[channel_name].get_topic().empty()){
                send_message(client.get_fd(), RPL_TOPIC(client.get_nickname(), channel_name, channels[channel_name].get_topic()));
                send_message(client.get_fd(), RPL_TOPICWHOTIME(client.get_nickname(), channel_name , channels[channel_name].get_topic_setter(), time_to_string(time_teller())));
            }
            send_names_list(client, channels[channel_name]);
        }
    }
    
  return ;  
}

};
