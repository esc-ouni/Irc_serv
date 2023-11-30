/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 10:20:22 by idouni            #+#    #+#             */
/*   Updated: 2023/11/30 17:28:11 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Irc.hpp"
#include "../headers/Channel.hpp"
#include "../headers/Client.hpp"
#include "../headers/commands.hpp"

void rtrim(std::string &s, const std::string& charsToTrim) {
    size_t endpos = s.find_last_not_of(charsToTrim);
    if (std::string::npos != endpos) {
        s = s.substr(0, endpos + 1);
    }
}

std::string extractChannelName(const std::string& command) {
    return (command.substr(command.find('#'), (command.find(' ') - command.find('#'))));
}

void sendMessage(int clientSocket, const std::string& message) {
    if (send(clientSocket, message.c_str(), message.length(), 0) == -1)
        std::cerr << "Err: failling sending message th the client !" << std::endl;
}

void Create_channel_join(int clientSocket, const std::string& username, const std::string& channelName) {
    // JOIN Confirmation
    std::string joinConfirm = ":" + username + "!" + username + "@" + "adress" + " JOIN :" + channelName + "\r\n";
    sendMessage(clientSocket, joinConfirm);


    // Channel Topic
    std::string topicMsg = ":MyServer 332 " + username + " " + channelName + " :" + "INTERESTING_TOPIC" + "\r\n";
    sendMessage(clientSocket, topicMsg);

    // Names List
    std::string userNames = "Ibraim Alexandra IMAD"; // This should return a space-separated list of nicknames
    std::string namesReply = ":MyServer 353 " + username + " = " + channelName + " :" + userNames + "\r\n";
    sendMessage(clientSocket, namesReply);

    // End of Names List
    std::string endOfNames = ":MyServer 366 " + username + " " + channelName + " :End of /NAMES list\r\n";
    sendMessage(clientSocket, endOfNames);

    std::cout << "Debug::Working... !" << std::endl;
}

// void handleJoinCommand(std::string command, Client &client, std::map<std::string, Channel> &channels) {

//     std::string channel_name = extractChannelName(command);
//     rtrim(channel_name, "\r\n");

//     channels[channel_name].addUser(client);

//     Create_channel_join(client.get_socket_fd(), client.get_nickname(), channel_name);

//     std::cout << "Channel " << channel_name << " created and " << client.get_nickname() << " joined." << std::endl;
    
// }

void handleJoinCommand(std::string command, Client &client, std::map<std::string, Channel>& channels) {
    std::string channel_name = extractChannelName(command);
    rtrim(channel_name, "\r\n");

    channels[channel_name].addUser(client);

    // Corrected JOIN confirmation message
    std::string joinConfirm = ":" + client.get_nickname() + "!~" + client.get_username() + "@" + "client.get_host()" + " JOIN :" + channel_name + "\r\n";
    sendMessage(client.get_socket_fd(), joinConfirm);

    // Assuming there's a topic; send a no-topic message if not
    std::string topicMsg = ":MyServer 332 " + client.get_nickname() + " " + channel_name + " :Some interesting topic\r\n";
    sendMessage(client.get_socket_fd(), topicMsg);

    // Send NAMES list
    std::string namesReply = ":MyServer 353 " + client.get_nickname() + " = " + channel_name + " :User1 User2 User3\r\n";
    sendMessage(client.get_socket_fd(), namesReply);

    // End of NAMES list
    std::string endOfNames = ":MyServer 366 " + client.get_nickname() + " " + channel_name + " :End of /NAMES list\r\n";
    sendMessage(client.get_socket_fd(), endOfNames);

    std::cout << "Channel " << channel_name << " created and " << client.get_nickname() << " joined." << std::endl;
}
