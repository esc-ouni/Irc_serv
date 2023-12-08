#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

void invite_user(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients){
    std::vector<std::string> args = parser(command, ' ');
    int argc = args.size();

    std::string user;
    std::string channel_name;

    if (argc == 3){
        //check if the client exist on the IRc
        //check if the channel exist
        //check if the excuter is a channel operator
        //check if the client not a member of that channel

        //successful invite
        // to the excuter :ServerName 341 InviterNick InvitedNick #Channel
        // to the invitee :InviterNick!InviterUser@InviterHost INVITE InvitedNick :#Channel
        // add to the invitee of the channel
        // channels[channel_name].add_to_invitee()

    }
    else 
        // BAD ARGS


    // DO CHECK WORK
    // if (!channel_exist(channels, channel_name)){
    //         std::cout << "ERR_NOSUCHCHANNEL" << std::endl;
    //         send_message(excuter.get_fd(), ERR_NOSUCHCHANNEL(excuter.get_nickname(), channel_name));
    //     return ;   
    // }
    // if (!excuter.is_operator(channels[channel_name])){
    //     send_message(excuter.get_fd(), ERR_CHANOPRIVSNEEDED(excuter.get_nickname(), channel_name));
    //     std::cout << "ERR_CHANOPRIVSNEEDED" << std::endl;
    //     return ;
    // }

    // INVITE THE USER
        // problem in the received command 
    return ;
}

