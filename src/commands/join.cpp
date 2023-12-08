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

bool channel_join(Client &client, std::map<std::string, Channel>& channels, std::string& channel_name, std::map<int, Client> &clients, std::string password){
    if (!channels[channel_name].add_user(client))
        return (false);
    return (true);
};

void send_names_list(Client &client, Channel &channel){
    send_message(client.get_fd(), RPL_NAMREPLY(client.get_nickname(), channel.get_name(), channel.get_all_users()));
    send_message(client.get_fd(), RPL_ENDOFNAMES(client.get_nickname(), channel.get_name()));
}


void handle_Join(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients) {
//    TBC
   
    // std::vector<std::string> args = parser(command, ' ');
    // int argc = args.size();

    // std::string         channel_name;
    // std::string         password;

    // if (argc == 3){
    //     while (std::getline(stream, channel_name, ',')) {
    //         if (!channel_name.empty() && channel_name_is_valid(channel_name)){
    //             if (!channel_exist(channels, channel_name)){
    //                 if(!Create_channel_join(client, channels, channel_name, clients))
    //                     return ;
    //             }
    //             else if (channel_exist(channels, channel_name)){
    //                 if(!channel_join(client, channels, channel_name, clients)){
    //                     send_message(client.get_fd(), ERR_CHANNELISFULL(client.get_nickname(), channels[channel_name].get_name()));
    //                     return ;
    //                 }
    //             }
    //             channels[channel_name].broadcast_message(RPL_NOTIFYJOIN(client.get_nickname(), channel_name));
    //             if (!channels[channel_name].get_topic().empty()){
    //                 send_message(client.get_fd(), RPL_TOPIC(client.get_nickname(), channel_name, channels[channel_name].get_topic()));
    //                 send_message(client.get_fd(), RPL_TOPICWHOTIME(client.get_nickname(), channel_name , channels[channel_name].get_topic_setter(), time_to_string(time_teller())));
    //             }
    //             send_names_list(client, channels[channel_name]);
    //         }
    //     }
    // }

};

