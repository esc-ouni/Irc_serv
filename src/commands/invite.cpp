#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"


int get_client_fd(std::string &client_name, std::map<int, Client> &clients){
    std::map<int, Client>::iterator it = clients.begin();

    while (it != clients.end()){
        if (client_name == it->second.get_nickname())
            return (it->second.get_fd());
        ++it;
    }
    return (0);
}


bool user_exist(std::string &client_name, std::map<int, Client> &clients){
    std::map<int, Client>::iterator it = clients.begin();

    while (it != clients.end()){
        if (client_name == it->second.get_nickname())
            return (true);
        ++it;
    }
    return (false);
}

void invite_user(std::string command, Client &excuter, std::map<std::string, Channel>& channels, std::map<int, Client> &clients){
    std::vector<std::string> args = parser(command, ' ');
    int argc = args.size();

    std::string user;
    std::string channel_name;

    if (argc == 3){
        user         = args[1];
        channel_name = args[2];
        //check if the excuter exist on the IRc
        if (!user_exist(user, clients)){
            send_message(excuter.get_fd(), ERR_NOSUCHNICK(excuter.get_nickname(), user));
            return ;
        }

        //check if the channel exist
        if (!channel_exist(channels, channel_name)){
                send_message(excuter.get_fd(), ERR_NOSUCHCHANNEL(excuter.get_nickname(), channel_name));
            return ;   
        }
        //check if the excuter is a channel operator
        if (!excuter.is_operator(channels[channel_name])){
            send_message(excuter.get_fd(), ERR_CHANOPRIVSNEEDED(excuter.get_nickname(), channel_name));
            return ;
        }

        //check if the client not a member of that channel
        if (channels[channel_name].is_member(user)){
            send_message(excuter.get_fd(), ERR_USERONCHANNEL(excuter.get_nickname(), user, channel_name));
            return ;
        }


        // add to the invitee of the channel
        if (channels[channel_name].add_to_invitee(clients[get_client_fd(user, clients)])){
            //successful invite
            // to the excuter :ServerName 341 InviterNick InvitedNick #Channel
            send_message(excuter.get_fd(), RPL_INVITESEND(excuter.get_nickname(), user, channel_name));
            
            // to the invitee :InviterNick!InviterUser@InviterHost INVITE InvitedNick :#Channel
            send_message(get_client_fd(user, clients), RPL_INVITENOTIFY(excuter.get_nickname(), user, channel_name));
        };
    }
    else 
        // BAD ARGS
    return ;
}

