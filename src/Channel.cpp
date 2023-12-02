/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 10:20:22 by idouni            #+#    #+#             */
/*   Updated: 2023/12/02 15:50:23 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Irc.hpp"
#include "../headers/Channel.hpp"
#include "../headers/Client.hpp"
#include "../headers/commands.hpp"


void Channel::addUser(Client &client) {
    this->_clients[client.get_fd()] = client;
};

std::string Channel::get_name() const{
    return (this->_name);
};

void Channel::set_name(Client &client, std::string &new_name){ // to mod
    this->_name = new_name;
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

void Channel::promote(Client &client){
    this->_operators[client.get_fd()] = client;
};

void Channel::unpromote(Client &client){
    if (is_operator(client)){
        this->_operators.erase(client.get_fd());
    };
};

void  Channel::set_topic(Client &client, std::string &new_topic){
    std::cout << std::boolalpha << is_operator(client) << std::endl;
    std::cout << std::boolalpha << is_valid_topic(new_topic) << std::endl;
    if (is_operator(client) /*&& this->_topic.compare(new_topic)*/ && is_valid_topic(new_topic)){
        this->_topic = new_topic;
        std::cout << "DEbug::Done !" << std::endl;
    }
};

std::string Channel::get_topic() const{
    return (this->_topic);
};

// Generally, channel names can include letters, digits, and some special characters.
// Typically allowed characters are: a-z, A-Z, 0-9, as well as underscores _, hyphens -, and periods .


bool channel_name_is_valid(std::string &channel_name){
    if (channel_name.empty() || (channel_name.length() > 50))
        return(false);
    if (channel_name.c_str()[0] != '#')
        return(false);
    for (int i = 0; i < channel_name.length(); i++){
        if (!std::isalnum(channel_name.c_str()[i]) || !valid_sp_character(channel_name.c_str()[i]) || std::isspace(channel_name.c_str()[i]))
            return(false);
    }
    return (true);
}

bool valid_sp_character(int c){
    if (c == '_' || c == '-' || c == '.')
        return (true);
    return (false);
} 

bool is_valid_topic(std::string &new_topic){
    if (new_topic.empty() || (new_topic.length() > 100))
        return(false);
    return (true);
}

void sendMessage(int clientSocket, const std::string& message) {
    if (send(clientSocket, message.c_str(), message.length(), 0) == -1)
        std::cerr << "Err: failling sending message th the client !" << std::endl;
};

std::string &trim(std::string &str, const std::string& charsToTrim) {
    size_t endpos = str.find_last_not_of(charsToTrim);

    if (std::string::npos != endpos) {
        str = str.substr(0, endpos + 1);
    }
    return (str);
};


void notifyUsersOfNewJoin(const std::string& channelName, Client& joiningClient, std::map<std::string, Channel>& channels, std::map<int, Client> &clients) {
    std::string joinMessage = ":" + joiningClient.get_nickname() + "!~" + joiningClient.get_nickname() + "@" + "client.get_host()" + " JOIN :" + channelName + "\r\n";

    //LOOP_OVER_CLENTS_SEND_TEM_NOTIFICATION
    std::map<int, Client>::iterator it = clients.begin();

    while (it != clients.end()){
        if (it->first != joiningClient.get_fd()){
            sendMessage(it->second.get_fd(), joinMessage);
        }
        ++it;
    }
};

void Create_channel_join(Client &client, std::map<std::string, Channel>& channels, const std::string& new_channel_name, std::map<int, Client> &clients) {
    // promotes the user to be an operator 
    channels[new_channel_name].promote(client);

    
    // Corrected JOIN confirmation message
    std::string joinConfirm = ":" + client.get_nickname() + "!~" + client.get_nickname() + "@" + "client.get_host()"+ " JOIN :" + new_channel_name + "\r\n"  + "\r\n";
    sendMessage(client.get_socket_fd(), joinConfirm);

    // Assuming there's a topic; send a no-topic message if not
    if (!channels[new_channel_name].get_topic().empty()){
        std::string topicMsg = ": 332 " + client.get_nickname() + " " + new_channel_name + " :" + channels[new_channel_name].get_topic() + "\r\n";
        sendMessage(client.get_socket_fd(), topicMsg);
        
    }

    // Send NAMES list
    std::string namesReply = ": 353 " + client.get_nickname() + " = " + new_channel_name + " :" + Get_Users_list(clients, channels[new_channel_name]) + "\r\n";
    sendMessage(client.get_socket_fd(), namesReply);

    // End of NAMES list
    std::string endofNames = ": 366 " + client.get_nickname() + " " + new_channel_name + " :End of /NAMES list\r\n";
    sendMessage(client.get_socket_fd(), endofNames);

    // std::cout << "Debug::Working... !" << std::endl;
};

std::string &extractChannelName(const std::string& command) {
    size_t n = command.find('#');
    std::string ch_name = "";
    
    if (n != std::string::npos) {
        ch_name = command.substr(n);
    }
    return (trim(ch_name, "\r\n"));
};


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
    Channel     channel;

    // channel.set_name()
    channels[channel_name] = channel;
    channels[channel_name].addUser(client);

    Create_channel_join(client, channels, channel_name, clients);
    notifyUsersOfNewJoin(channel_name, client, channels, clients);


    // std::cout << "DEBUG::Channel " << channel_name << " created and " << client.get_nickname() << " joined." << std::endl;
}

std::string extracTopic(std::string& command) {
    std::string topic = "";
    size_t n = command.find('#');    
    
    if (n != std::string::npos) {
        n = command.find(' ', n);
        if (n != std::string::npos) {
            topic = command.substr(n + 1);
            if (!topic.empty() && !topic.compare(0, 1, ":")){
                topic.erase(0, 1);
            }
        }
    }
    return (trim(topic, "\r\n"));
}

void set_topic(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients){
    std::string topic;

    std::cout << std::boolalpha << client.is_operator(channels["#Rooom"]) << std::endl<< std::endl;
    topic = extracTopic(command);
    std::cout << " :"<< extractChannelName(command) <<": " << std::endl<< std::endl;
 
    if (!topic.empty()){
        client.set_topic(channels[extractChannelName(command)], topic);
        sendMessage(client.get_fd(), RPL_TOPIC(client.get_nickname(), extractChannelName(command), channels[extractChannelName(command)].get_topic())); 
        // std::cout << "DEBUG::SET_TOPIC command <"<< command <<"> <" << topic << ">" << std::endl;
    }
    else{
        sendMessage(client.get_fd(), RPL_TOPIC(client.get_nickname(), extractChannelName(command), channels[extractChannelName(command)].get_topic()));
        if (!channels[extractChannelName(command)].get_topic().empty()){
            std::string reply = ": 333 " + client.get_nickname() + " " + extractChannelName(command) + " Somonee 1972531900\r\n";
            // std::string reply = RPL_TOPICWHOTIME(client.get_nickname(), extractChannelName(command), getCurrentTime());
            sendMessage(client.get_fd(), reply); 
            
        }
        std::cout << "DEBUG::SHOW_TOPIC <" << channels[extractChannelName(command)].get_topic() << ">" << std::endl;
    }
};