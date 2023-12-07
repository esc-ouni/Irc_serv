#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

bool is_valid_topic(std::string &new_topic){
    if (new_topic.empty() || (new_topic.length() > 100))
        return(false);
    return (true);
}

void set_topic(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients){
    std::string topic = extract_topic(command);
    std::string channel_name = extract_channel_name(command);

    if(!channel_exist(channels, channel_name)){
        send_message(client.get_fd(), ERR_NOSUCHCHANNEL(client.get_nickname(), channel_name));
        return ;
    }
    if (!topic.empty()){
        if (client.set_topic(channels[channel_name], topic)){
            channels[channel_name].broadcast_message(RPL_TOPICCHANGE(client.get_nickname(), channel_name, channels[channel_name].get_topic()));
        }
    }
    else{
        if (!channels[channel_name].get_topic().empty()){
            send_message(client.get_fd(), RPL_TOPIC(client.get_nickname(), channel_name, channels[channel_name].get_topic())); 
            send_message(client.get_fd(), RPL_TOPICWHOTIME(client.get_nickname(), channel_name , channels[channel_name].get_topic_setter(), time_to_string(time_teller())));
        }
        else if (channels[channel_name].get_topic().empty()){
            send_message(client.get_fd(), RPL_TOPIC(client.get_nickname(), channel_name, channels[channel_name].get_topic()));
        }
    }
};
