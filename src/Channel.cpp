/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 10:20:22 by idouni            #+#    #+#             */
/*   Updated: 2023/11/29 14:44:41 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Irc.hpp"
#include "../headers/Channel.hpp"
#include "../headers/Client.hpp"
#include "../headers/commands.hpp"


std::string extractChannelName(const std::string& command) {
    return (command.substr(command.find('#') + 1, command.find(' ', command.find('#'))- (command.find('#') + 1)));
}

void handleJoinCommand(std::string command, Client &client, std::map<std::string, Channel> &channels) {

    std::string channel_name = extractChannelName(command);

    channels[channel_name].addUser(client);


    // std::string RET = ":" + client.get_username() + " JOIN " + channel_name + "\r\n";
    std::string RET = ":" + RPL_JOIN(client.get_username(), client, channel_name)+ "\r\n";
    send(client.get_socket_fd(), RET.c_str(), RET.length(), 0);


    std::cout << "Channel " << channel_name << " created and " << client.get_nickname() << " joined." << std::endl;
    
}