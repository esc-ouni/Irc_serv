/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 10:20:22 by idouni            #+#    #+#             */
/*   Updated: 2023/11/30 19:20:36 by idouni           ###   ########.fr       */
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

// void Create_channel_join(int clientSocket, const std::string& username, const std::string& channelName) {


//     std::cout << "Debug::Working... !" << std::endl;
// }
std::string extractChannelName(const std::string& command) {
    return (command.substr(command.find('#'), (command.find(' ') - command.find('#'))));
}

void sendMessage(int clientSocket, const std::string& message) {
    if (send(clientSocket, message.c_str(), message.length(), 0) == -1)
        std::cerr << "Err: failling sending message th the client !" << std::endl;
}

std::string Get_Users_list(std::map<int, Client> &clients){

    std::map<int, Client>::iterator it = clients.begin();

    while (it != clients.end()) {
        std::cout << "Fd: " << it->first << ", CLient Name: " << it->second.get_nickname() << std::endl;
        ++it;
    }
    return "";
        
};

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

    
    std::string namesReply = ":MyServer 353 " + client.get_nickname() + " = " + channel_name + " :" + "@Ibrahim @ALEXANDRA @Imad" + "\r\n";
    sendMessage(client.get_socket_fd(), namesReply);

    // End of NAMES list
    std::string endofNames = ":MyServer 366 " + client.get_nickname() + " " + channel_name + " :End of /NAMES list\r\n";
    sendMessage(client.get_socket_fd(), endofNames);

    std::cout << "DEBUG::Channel " << channel_name << " created and " << client.get_nickname() << " joined." << std::endl;
}
