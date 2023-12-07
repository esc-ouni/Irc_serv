#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

void leave_channel(std::string command, Client &client, std::map<std::string, Channel>& channels){
    std::string channel_name = extract_channel_name(command);

    if(!channel_exist(channels, channel_name)){
        return ; 
    }
    if (!channels[channel_name].is_member(client)){
        return ; 
    }
    // departure notify 
    channels[channel_name].broadcast_message(RPL_NOTIFYPART(client.get_nickname(), channel_name));
    
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
