/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 10:20:22 by idouni            #+#    #+#             */
/*   Updated: 2023/12/03 20:47:06 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Irc.hpp"
#include "../headers/Channel.hpp"
#include "../headers/Client.hpp"
#include "../headers/commands.hpp"


void Channel::broadcast_message(std::string &message){
    
    std::map<int, Client>::iterator it = this->_clients.begin();

    while (it != this->_clients.end()){
        sendMessage(it->first, message);
        ++it;
    }
};

std::string Channel::get_all_users(){
    std::string                     ALL_USERS = "";
    std::map<int, Client>::iterator it = this->_clients.begin();
    
    while (it != this->_clients.end()) {
        if (it->second.is_operator((*this)))
            ALL_USERS += "@" + it->second.get_nickname() + " ";
        else
            ALL_USERS += it->second.get_nickname() + " ";
        it++;
    }
    return ALL_USERS;
}


void Channel::broadcast_message_exp(Client &client, std::string &message){
    
    std::map<int, Client>::iterator it = this->_clients.begin();

    while (it != this->_clients.end()){
        if (it->first != client.get_fd())
            sendMessage(it->first, message);
        ++it;
    }
};


std::string Channel::get_topic_setter() const{
    return (this->_topic_setter);
};

std::string Channel::get_topic_date() const{
    return (this->_topic_date);
};

time_t time_teller(){
    time_t      current_time = time(NULL);
    return (current_time);
}

std::string timeToString(time_t timeVal) {
    std::ostringstream oss;
    oss << timeVal;
    return oss.str();
}

void Channel::remove_user(Client &client) {
    if (this->is_member(client)){
        if (client.is_operator((*this)))
            this->unpromote(client);
        this->_clients.erase(client.get_fd());  
    }
};

void Channel::add_user(Client &client) {
    this->_clients[client.get_fd()] = client;
};

std::string Channel::get_name() const{
    return (this->_name);
};

void Channel::set_name(std::string &new_name){ // IMADD
    this->_name = new_name;
};

bool Channel::is_member(Client &client){
    std::map<int, Client>::iterator it = _clients.begin();

    while (it != _clients.end()){
        if (it->first == client.get_fd()){
            return (true);
        }
        it++;
    }
    return (false);
};

bool Channel::is_operator(Client &client){
    std::map<int, Client>::iterator it = _operators.begin();

    while (it != _operators.end()){
        if (it->first == client.get_fd()){
            return (true);
        }
        it++;
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

bool  Channel::set_topic(Client &client, std::string &new_topic){    
    if (is_operator(client) && this->_topic.compare(new_topic) && is_valid_topic(new_topic)){
        this->_topic        = new_topic;
        this->_topic_setter = client.get_nickname();
        this->_topic_date   = timeToString(time_teller());
        return (true);
    }
    return (false);
};

std::string Channel::get_topic() const{
    return (this->_topic);
};




void leave_channel(std::string command, Client &client, std::map<std::string, Channel>& channels){
    std::string channel_name = extractChannelName(command);

    if(!channel_exist(channels, channel_name)){
        //NO_CHANNEL
        return ; 
    }
    if (!channels[channel_name].is_member(client)){
        //CLIENT_NOT_A_CHANNEL_MEMBER
        // RR_NOTONCHANNEL (442)  :ServerName 442 ClientNick #channel :You're not on that channel
        return ; 
    }
    // departure notify 
    std::string Reply = RPL_NOTIFYPART(client.get_nickname(), channel_name);
    channels[channel_name].broadcast_message(Reply);
    
    // remove client from channel
    channels[channel_name].remove_user(client);
    
    
};


void quit_server(Client &client, std::map<int, Client> &clients, std::map<std::string, Channel>& channels){
    std::map<std::string, Channel>::iterator it = channels.begin();
    std::string quit_message = RPL_NOTIFYQUIT(client.get_nickname(), "SEE YALL");

    
    
    // quit notify 
    while (it != channels.end()){
        if (channels[it->first].is_member(client)){
            channels[it->first].broadcast_message_exp(client, quit_message);
            channels[it->first].remove_user(client);
        }
        it++;
    }
    // quit
    // clients.erase(client.get_fd());
};


void kick_user(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients){
    std::string channel_name = extractChannelName(command);

    // get target client
    // extract reason from command

    
    if(!channel_exist(channels, channel_name)){
        //NO_CHANNEL
        return ; 
    }
    if (!channels[channel_name].is_member(client)){
        //CLIENT_NOT_A_CHANNEL_MEMBER
        // RR_NOTONCHANNEL (442)  :ServerName 442 ClientNick #channel :You're not on that channel
        return ; 
    }
    // if (!channels[channel_name].is_member(target)){
    //     //TARGET_NOT_A_CHANNEL_MEMBER
    //     // ERR_USERNOTINCHANNEL (441)
    //     return ; 
    // }
    if (!channels[channel_name].is_operator(client)){
        //CLIENT_HAS_NO_PRIVELLIGES
        // ERR_CHANOPRIVSNEEDED (482)
        return ; 
    }
    std::string kick_message = RPL_KICK(client.get_nickname(), channel_name, "test", "istighlal_solta");
    
    // kick the user
    channels[channel_name].broadcast_message(kick_message);
    
    // channels[channel_name].remove_user(target)
    
};


bool channel_name_is_valid(std::string &channel_name){
    if (channel_name.empty() || (channel_name.length() > 50))
        return(false);
    if (channel_name.c_str()[0] != '#')
        return(false);
    for (int i = 1; i < channel_name.length(); i++){
        if ((!std::isalnum(channel_name.c_str()[i]) && !valid_sp_character(channel_name.c_str()[i])) || std::isspace(channel_name.c_str()[i]))
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

std::string trim(std::string &str, const std::string& charsToTrim) {
    size_t endpos = str.find_last_not_of(charsToTrim);

    if (std::string::npos != endpos) {
        str = str.substr(0, endpos + 1);
    }
    return (str);
};

std::string extractChannelName(const std::string& command) {
    size_t n = command.find('#');
    std::string ch_name = "";
    
    if (n != std::string::npos) {
        ch_name = command.substr(n);
        n = ch_name.find(' ');
        if (n != std::string::npos){
            ch_name = ch_name.substr(0, n);
        }
    }
    return (trim(ch_name, "\r\n"));
};

bool channel_exist(std::map<std::string, Channel>& channels, std::string &needle){
    std::map<std::string, Channel>::iterator it = channels.begin();
    
    while (it != channels.end()){
        if (!needle.compare(it->first))
            return (true);
        it++;
    }
    return (false);
    
}

void Create_channel_join(Client &client, std::map<std::string, Channel>& channels, std::string& new_channel_name, std::map<int, Client> &clients) {
    channels[new_channel_name].set_name(new_channel_name);
    channels[new_channel_name].add_user(client);
    
    // promotes the user to be an operator 
    channels[new_channel_name].promote(client);
};

void channel_join(Client &client, std::map<std::string, Channel>& channels, std::string& channel_name, std::map<int, Client> &clients){
    channels[channel_name].add_user(client);
};

void send_names_list(Client &client, Channel &channel){
    // Send NAMES list
    std::string Message = RPL_NAMREPLY(client.get_nickname(), channel.get_name(), channel.get_all_users());
    sendMessage(client.get_fd(), Message);

    // End of NAMES list
    Message = RPL_ENDOFNAMES(client.get_nickname(), channel.get_name());
    sendMessage(client.get_fd(), Message);
}


void handleJoinCommand(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients) {



    std::string         input = extractChannelName(command);
    std::istringstream  iss(input);
    std::string         channel_name;
    std::string         Message;

    // std::cout << "INPUT : <" << input << "> " << std::endl;
    while (std::getline(iss, channel_name, ',')) {
        // std::cout << "part : <" << channel_name << "> \t\t" << std::boolalpha << channel_name_is_valid(channel_name) << std::endl;
        
        if (!channel_name.empty() && channel_name_is_valid(channel_name)){
            if (!channel_exist(channels, channel_name)){
                Create_channel_join(client, channels, channel_name, clients);
            }
            else if (channel_exist(channels, channel_name)){
                channel_join(client, channels, channel_name, clients);
            }
            // notifyUsersOfNewJoin
            Message = RPL_NOTIFYJOIN(client.get_nickname(), channel_name);
            channels[channel_name].broadcast_message(Message);
            
            if (!channels[channel_name].get_topic().empty()){
                // RPL_TOPIC
                sendMessage(client.get_fd(), RPL_TOPIC(client.get_nickname(), channel_name, channels[channel_name].get_topic()));
                // RPL_TOPICWHOTIME
                sendMessage(client.get_fd(), RPL_TOPICWHOTIME(client.get_nickname(), channel_name , channels[channel_name].get_topic_setter(), timeToString(time_teller())));
            }
            send_names_list(client, channels[channel_name]);
            
            // std::cout << "Channel : <"<< channel_name <<">\t, List of users : <" << channels[channel_name].get_all_users() << ">"<< std::endl;
        }
    }
};


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
    std::string topic = extracTopic(command);
    std::string channel_name = extractChannelName(command);

//  Channel Existence Check
    if(!channel_exist(channels, channel_name)){
        sendMessage(client.get_fd(), ERR_NOSUCHCHANNEL(client.get_nickname(), channel_name));
        return ;
    }

    if (!topic.empty()){
        if (client.set_topic(channels[channel_name], topic)){
            // Broadcast Topic Change
            std::string br = RPL_TOPICCHANGE(client.get_nickname(), channel_name, channels[channel_name].get_topic());
            channels[channel_name].broadcast_message(br);
        }
    }
    else{
        if (!channels[channel_name].get_topic().empty()){
            sendMessage(client.get_fd(), RPL_TOPIC(client.get_nickname(), channel_name, channels[channel_name].get_topic())); 
            sendMessage(client.get_fd(), RPL_TOPICWHOTIME(client.get_nickname(), channel_name , channels[channel_name].get_topic_setter(), timeToString(time_teller())));
        }
        else if (channels[channel_name].get_topic().empty()){
            sendMessage(client.get_fd(), RPL_TOPIC(client.get_nickname(), channel_name, channels[channel_name].get_topic()));
        }
    }
};