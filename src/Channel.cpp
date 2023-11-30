/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 10:20:22 by idouni            #+#    #+#             */
/*   Updated: 2023/11/29 19:44:57 by idouni           ###   ########.fr       */
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


    std::string RET    = ":" + RPL_JOIN(client.get_username(), client, channel_name) + "\r\n";
    send(client.get_socket_fd(), RET.c_str(), RET.length(), 0);


    std::string USERS = ": 354 hober 743 " + channel_name + " ~quassel" + " Christian_Qs H 0 0 :Christian" + "\r\n";
    send(client.get_socket_fd(), USERS.c_str(), USERS.length(), 0);

    std::string END =  ": 315 Me " + channel_name + " :End of /NAME list" + "\r\n";
    send(client.get_socket_fd(), END.c_str(), END.length(), 0);

    std::cout << "Channel " << channel_name << " created and " << client.get_nickname() << " joined." << std::endl;
    
}


// #include <iostream>
// #include <string>
// #include <map>
// // ... include other necessary headers ...

// void handleJoinCommand(std::string command, Client &client, std::map<std::string, Channel> &channels) {
//     std::string channel_name = extractChannelName(command);
//     channels[channel_name].addUser(client);

//     std::string joinConfirm = ": " + client.get_username() + " JOIN " + channel_name + "\r\n";
//     std::cout << "Sending JOIN confirmation: " << joinConfirm;
//     send(client.get_socket_fd(), joinConfirm.c_str(), joinConfirm.length(), 0);

//     std::string namesReply = ": 353 " + client.get_username() + " = " + channel_name + " #Ibrahim &Alexandra :Imad\r\n";
//     std::cout << "Sending NAMES list: " << namesReply;
//     send(client.get_socket_fd(), namesReply.c_str(), namesReply.length(), 0);

//     std::string endOfNames = ": 366 " + client.get_username() + " " + channel_name + " :End of /NAMES list\r\n";
//     std::cout << "Sending end of NAMES list: " << endOfNames;
//     send(client.get_socket_fd(), endOfNames.c_str(), endOfNames.length(), 0);

//     std::cout << "Channel " << channel_name << " created and " << client.get_nickname() << " joined." << std::endl;
// }


