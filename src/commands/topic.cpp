#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

bool is_valid_topic(std::string &new_topic){
    if (new_topic.empty() || (new_topic.length() > 100))
        return(false);
    return (true);
}

void set_topic(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients){
    std::vector<std::string> args = parser(command, ' ');
    int argc = args.size();

    std::string              channel_name;
    std::string              topic = "";

    if (argc >= 2){
        if (argc >= 3)
            topic = extract_topic(command);
        channel_name = args[1];

    if(!channel_exist(channels, channel_name)){
        send_message(client.get_fd(), ERR_NOSUCHCHANNEL(client.get_nickname(), channel_name));
        return ;
    }
    if (!channels[channel_name].is_member(client)){
        send_message(client.get_fd(), ERR_NOTONCHANNEL(client.get_nickname(), channel_name));
        return ;
    }
    if (channels[channel_name].get_option_t()){
        if (!channels[channel_name].is_operator(client)){
            send_message(client.get_fd(), ERR_CHANOPRIVSNEEDED(client.get_nickname(), channel_name));
            return ;
        }
    }
    if (!topic.empty()){
        if (client.set_topic(channels[channel_name], topic)){
            channels[channel_name].broadcast_message(RPL_TOPICCHANGE(client.get_nickname(), client.get_client_host(), channel_name, channels[channel_name].get_topic()));
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
    }
    else
        send_message(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "TOPIC"));
    return ;

};
