#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

void kick_user(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients){
    std::vector<std::string> args = parser(command, ' ');
    int argc = args.size();

    std::string              user;
    std::string              channel_name;
    int                      target_fd = 0;
    std::string              reason;

    if (argc >= 3){
        channel_name = args[1];
        user         = args[2];
        reason       = extract_reason(command);

        // check channel existence
        if(!channel_exist(channels, channel_name)){
            send_message(client.get_fd(), ERR_NOSUCHCHANNEL(client.get_nickname(), channel_name));
            return ; 
        }
                
        // check kicker permission
        if (!channels[channel_name].is_operator(client)){
            send_message(client.get_fd(), ERR_CHANOPRIVSNEEDED(client.get_nickname(), channel_name));
            return ; 
        }
        
        // check target client
        target_fd = channels[channel_name].is_member(user);
        if (!target_fd){
            send_message(client.get_fd(), ERR_USERNOTINCHANNEL(client.get_nickname(), user, channel_name));
            return ; 
        }

        std::string kick_message = RPL_KICK(client.get_nickname(), client.get_client_host(), channel_name, clients[target_fd].get_nickname() , reason);
        
        // // kick the user
        channels[channel_name].broadcast_message(kick_message);            
        channels[channel_name].remove_user(clients[target_fd]);
    }
    else
        send_message(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "KICK"));
};

