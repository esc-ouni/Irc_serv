#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

void leave_channel(std::string command, Client &client, std::map<std::string, Channel>& channels){
    std::string channel_name = extract_channel_name(command);

    if(!channel_exist(channels, channel_name)){
        send_message(client.get_fd(), ERR_NOSUCHCHANNEL(client.get_nickname(), channel_name));
        return ; 
    }
    if (!channels[channel_name].is_member(client)){
        send_message(client.get_fd(), ERR_NOTONCHANNEL(client.get_nickname(), channel_name));
        return ; 
    }
    channels[channel_name].broadcast_message(RPL_NOTIFYPART(client.get_nickname(), client.get_client_host(), channel_name));
    channels[channel_name].remove_user(client);    
};


void quit_server(Client &client, std::map<int, Client> &clients, std::map<std::string, Channel>& channels){
    std::map<std::string, Channel>::iterator it = channels.begin();
    
    while (it != channels.end()){
        if (it->second.is_member(client)){
            it->second.broadcast_message_exp(client, RPL_NOTIFYQUIT(client.get_nickname(), client.get_client_host(), "SEE YALL"));
            it->second.remove_user(client);
        }
        else if (it->second.is_invited(client))
            it->second.remove_from_invite_list(client);
        it++;
    }
};
