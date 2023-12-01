/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 10:20:22 by idouni            #+#    #+#             */
/*   Updated: 2023/12/01 19:37:38 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Irc.hpp"
#include "../headers/Channel.hpp"
#include "../headers/Client.hpp"
#include "../headers/commands.hpp"


void Channel::addUser(Client &client) {
    this->_clients[client.get_fd()] = client;
};

bool Channel::is_operator(Client &client){
    std::map<int, Client>::iterator it = _operators.begin();

    while (it != _operators.end()){
        if (it->first == client.get_fd()){
            return (true);
        }
    }
    return (false);
};

// void Channel::promote(Client &client){
//     this->_operators[1].second = client;
// };


// Generally, channel names can include letters, digits, and some special characters.
// Typically allowed characters are: a-z, A-Z, 0-9, as well as underscores _, hyphens -, and periods .

// bool channel_name_is_valid(){
    
//     return(true);
// }


void sendMessage(int clientSocket, const std::string& message) {
    if (send(clientSocket, message.c_str(), message.length(), 0) == -1)
        std::cerr << "Err: failling sending message th the client !" << std::endl;
};

void rtrim(std::string &s, const std::string& charsToTrim) {
    size_t endpos = s.find_last_not_of(charsToTrim);
    if (std::string::npos != endpos) {
        s = s.substr(0, endpos + 1);
    }
}


void notifyUsersOfNewJoin(const std::string& channelName, Client& joiningClient, std::map<std::string, Channel>& channels) {
    Channel& channel = channels[channelName];
    std::string joinMessage = ":" + joiningClient.get_nickname() + "!~" + "joiningClient.get_username()" + "@" + "client.get_host()" + " JOIN :" + channelName + "\r\n";


    //LOOP_OVER_CLENTS_SEND_TEM_NOTIFICATION
    sendMessage(joiningClient.get_socket_fd(), joinMessage);
}




void Create_channel_join(Client &client, std::map<std::string, Channel>& channels, const std::string& new_channel_name, std::map<int, Client> &clients) {
    // client.is_operator() = True;
    
    
    // Corrected JOIN confirmation message
    std::string joinConfirm = ":" + client.get_nickname() + "!~" + client.get_nickname() + "@" + "client.get_host()"+ " JOIN :" + new_channel_name + "\r\n"  + "\r\n";
    sendMessage(client.get_socket_fd(), joinConfirm);

    // Assuming there's a topic; send a no-topic message if not
    std::string topicMsg = ": 332 " + client.get_nickname() + " " + new_channel_name + " :Some interesting topic\r\n";
    sendMessage(client.get_socket_fd(), topicMsg);

    // Send NAMES list
    std::string namesReply = ": 353 " + client.get_nickname() + " = " + new_channel_name + " :" + Get_Users_list(clients, channels[new_channel_name]) + "\r\n";
    sendMessage(client.get_socket_fd(), namesReply);

    // End of NAMES list
    std::string endofNames = ": 366 " + client.get_nickname() + " " + new_channel_name + " :End of /NAMES list\r\n";
    sendMessage(client.get_socket_fd(), endofNames);

    // std::cout << "Debug::Working... !" << std::endl;
}

std::string extractChannelName(const std::string& command) {
    return (command.substr(command.find('#'), (command.find(' ') - command.find('#'))));
}


std::string Get_Users_list(std::map<int, Client> &clients, Channel &channel){

    std::map<int, Client>::iterator it = clients.begin();
    std::string                     ALL_USERS = "";
    
    while (it != clients.end()) {
        if (it->second.is_operator(channel))
            ALL_USERS += "@" + it->second.get_nickname() + " ";
        else
            ALL_USERS += it->second.get_nickname() + " ";
        ++it;
    }
    return ALL_USERS;
        
};

void handleJoinCommand(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients) {
    std::string channel_name = extractChannelName(command);
    rtrim(channel_name, "\r\n");

    channels[channel_name].addUser(client);

    Create_channel_join(client, channels, channel_name, clients);
    // notifyUsersOfNewJoin(channel_name, client, channels);



    std::cout << "DEBUG::Channel " << channel_name << " created and " << client.get_nickname() << " joined." << std::endl;
}
