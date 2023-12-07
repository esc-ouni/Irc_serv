#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

void kick_user(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients){
    // extract reason from command
    std::string reason =       extract_reason(command);
    
    std::string target =       extract_target(command);
    std::string channel_name = extract_channel_name(command);
    int         target_fd = 0;

    // check channel existence
    if(!channel_exist(channels, channel_name)){
        //NO_CHANNEL
        return ; 
    }
    
    // check kicker client
    if (!channels[channel_name].is_member(client)){
        //CLIENT_NOT_A_CHANNEL_MEMBER
        // RR_NOTONCHANNEL (442)  :ServerName 442 ClientNick #channel :You're not on that channel
        return ; 
    }
    
    // check kicker permission
    if (!channels[channel_name].is_operator(client)){
        //CLIENT_HAS_NO_PRIVELLIGES
        // ERR_CHANOPRIVSNEEDED (482)
        send_message(client.get_fd(), ERR_CHANOPRIVSNEEDED(client.get_nickname(), channel_name));
        return ; 
    }
    
    // check target client
    target_fd = channels[channel_name].is_member(target);
    if (!target_fd){
        //TARGET_NOT_A_CHANNEL_MEMBER
        // RR_NOTONCHANNEL (442)  :ServerName 442 ClientNick #channel :You're not on that channel
        return ; 
    }

    
    std::string kick_message = RPL_KICK(client.get_nickname(), channel_name, clients[target_fd].get_nickname() , reason);
    
    // // kick the user
    channels[channel_name].broadcast_message(kick_message);
        
    channels[channel_name].remove_user(clients[target_fd]);
};

