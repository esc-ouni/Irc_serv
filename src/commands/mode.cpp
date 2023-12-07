/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 14:17:20 by idouni            #+#    #+#             */
/*   Updated: 2023/12/07 11:32:57 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"


long int contains_only_nums(std::string string){
    if (string.empty())
        return (-1);
    for (size_t i = 0; i < string.length(); i++){
        if (string.c_str()[0] == '+')
            continue ;
        if (!isdigit(string.c_str()[i]) || i == 10)
            return (-1);
    }
    return (std::atol(string.c_str()));
};

std::vector<std::string> parser(std::string &full_command, char dilimeter){
    std::vector<std::string> args;
    std::stringstream        stream(full_command);
    std::string              token;
    
    if (full_command.empty())
        return args;
    while (!std::getline(stream , token,  dilimeter).eof()){
        args.push_back(token);
        token.clear();
    }
    if (!token.empty()){
        args.push_back(trim(token, "\r\n"));
    }
    return args;
};


bool    valid_option(std::string &option){
    if (option.length() != 2)
        return (false);        
    if ((option.at(0) != '+' && option.at(0) != '-'))
        return (false);
    if ((option.at(1) != 'i' && option.at(1) != 't' && option.at(1) != 'k' && option.at(1) != 'o' && option.at(1) != 'l'))
        return (false);     
    return (true);
};

void mode_one_param(){
    std::cout << "Reply(650): MODE <target> [[(+|-)]<modes> [<mode-parameters>]]" << std::endl;
    // Reply(650): MODE <target> [[(+|-)]<modes> [<mode-parameters>]]
};

void mode_two_params(std::map<std::string, Channel>& channels, Client &excuter, std::string &channel_name){
    if (!channel_exist(channels, channel_name)){
        std::cout << "ERR_NOSUCHCHANNEL" << std::endl;
        //  ERR_NOSUCHCHANNEL (403)
        return ;
    }
    else if (!channels[channel_name].is_member(excuter)){
        std::cout << "ERR_NOTAMEMBER" << std::endl;
        //  ERR_NOTONCHANNEL (482)    
        return ;   
    }
    else{
        std::string notice;
        if (!channels[channel_name].show_mode().empty()){
            //  RPL_CHANNELMODEIS (324)
            notice = RPL_CHANNELMODEIS(excuter.get_nickname(), channel_name, channels[channel_name].show_mode());
            sendMessage(excuter.get_fd(), notice);
        }
        notice = RPL_CREATIONTIME(excuter.get_nickname(), channel_name, channels[channel_name].get_creation_date());
        sendMessage(excuter.get_fd(), notice);
        //  RPL_CREATIONTIME (329)
        std::cout << "SHOW AVAILABLE MODS ON THIS CHANNEL !" << std::endl;
        std::cout << "CHANNEL mode : " << channels[channel_name].show_mode() << std::endl;
        return ;
    }
};

void mode_three_params(std::map<std::string, Channel>& channels, Client &excuter, std::string &channel_name, std::string &mode){
    if (!channel_exist(channels, channel_name)){
        std::cout << "ERR_NOSUCHCHANNEL" << std::endl;
        //  ERR_NOSUCHCHANNEL (403)
        return ;   
    }
    if (!excuter.is_operator(channels[channel_name])){
        std::cout << "ERR_CHANOPRIVSNEEDED" << std::endl;
        //  ERR_CHANOPRIVSNEEDED (482)
        return ;
    }    
    if (!valid_option(mode)){
        std::cout << "ERR_UMODEUNKNOWNFLAG" << std::endl;
        // ERR_UMODEUNKNOWNFLAG (501)
        return ;
    }
    if (mode.at(1) == 'k' || mode.at(1) == 'o' || (mode.at(1) == 'l' && mode.at(0) == '+')){
        std::cout << "ERR_INVALIDMODEPARAM" << std::endl;
        // ERR_INVALIDMODEPARAM (696)
        return ;
    }
    if (mode.at(1) == 'i'){
        if (mode.at(0) == '+' && !channels[channel_name].get_option_i()){
            std::cout << "MODE #Channel +i" << std::endl;
            // broadcast the mode change to all channel users
            // RP => :UserNick!UserHost MODE #Channel +i
            // channels[channel_name].broadcast_message();
            channels[channel_name].set_option_i(true);
            return ;
        }
        if (mode.at(0) == '-' && channels[channel_name].get_option_i()){
            std::cout << "MODE #Channel -i" << std::endl;
            // broadcast the mode change to all channel users
            // RP => :UserNick!UserHost MODE #Channel -i
            // channels[channel_name].broadcast_message();
            channels[channel_name].set_option_i(false);
            return ;
        }
        return ;
    }
    else if (mode.at(1) == 't'){
        if (mode.at(0) == '+' && !channels[channel_name].get_option_t()){
            std::cout << "MODE #Channel +t" << std::endl;
            // broadcast the mode change to all channel users
            // RP => :UserNick!UserHost MODE #Channel +t
            // channels[channel_name].broadcast_message();
            channels[channel_name].set_option_t(true);
            return ;
        }
        else if (mode.at(0) == '-' && channels[channel_name].get_option_t()){
            std::cout << "MODE #Channel -t" << std::endl;
            // broadcast the mode change to all channel users
            // RP => :UserNick!UserHost MODE #Channel -t
            // channels[channel_name].broadcast_message();
            channels[channel_name].set_option_t(false);
            return ;
        }
        return ;
    }
    if (mode.at(1) == 'l' && mode.at(0) == '-' && channels[channel_name].get_option_l()){
        channels[channel_name].set_limit(CHANNEL_LIMIT);
        std::cout << "MODE #Channel -l" << std::endl;
        channels[channel_name].set_option_l(false);
        // :UserNick!UserHost MODE #Channel -l
        return ;
    }
};

void mode_four_params(std::map<std::string, Channel>& channels, Client &excuter, std::string &channel_name, std::string &mode, std::string &last_param){
    if (!channel_exist(channels, channel_name)){
            std::cout << "ERR_NOSUCHCHANNEL" << std::endl;
        //  ERR_NOSUCHCHANNEL (403)
        return ;   
    }
    if (!excuter.is_operator(channels[channel_name])){
        std::cout << "ERR_CHANOPRIVSNEEDED" << std::endl;
        //  ERR_CHANOPRIVSNEEDED (482)
        return ;
    }    
    if (!valid_option(mode)){
        std::cout << "ERR_UMODEUNKNOWNFLAG" << std::endl;
        // ERR_UMODEUNKNOWNFLAG (501)
        return ;
    }
    if (mode.at(1) == 'i' || mode.at(1) == 't'){
        std::cout << "ERR_INVALIDMODEPARAM" << std::endl;
        // ERR_INVALIDMODEPARAM (696)
        return ;
    }
    if (mode.at(1) == 'k'){
        if (mode.at(0) == '+' && !channels[channel_name].get_option_k()){
            if (!is_valid_password(last_param)){
                std::cout << "ERR_INVALIDKEY" << std::endl;
                // ERR_INVALIDKEY (525)
                return ;
            }
            if (channels[channel_name].set_password(last_param)){
                std::cout << "MODE #Channel +k key" << std::endl;
                // broadcast the mode change to all channel users
                // :UserNick!UserHost MODE #Channel +k key
                // channels[channel_name].broadcast_message();
                channels[channel_name].set_option_k(true);                
                channels[channel_name].lock();
            }
            return ;
        }
        else if (mode.at(0) == '-' && channels[channel_name].get_option_k()){
            if (last_param == channels[channel_name].get_password()){
                std::cout << "MODE #Channel -k key" << std::endl;
                // broadcast the mode change to all channel users
                // :UserNick!UserHost MODE #Channel -k key
                // channels[channel_name].broadcast_message();
                channels[channel_name].set_option_k(false);
                channels[channel_name].unlock();
            }
            return ;
        }
        return ;
    }
    else if (mode.at(1) == 'l'){
        if (mode.at(0) == '+' && !channels[channel_name].get_option_l()){
            if (contains_only_nums(last_param) != -1){
                if (channels[channel_name].set_limit(contains_only_nums(last_param))){
                    std::cout << "MODE #Channel +l number" << std::endl;
                    // :UserNick!UserHost MODE #Channel +l number
                    channels[channel_name].set_option_l(true);
                }
            }
        }
        return ;
    }        
};



//IMPLEMENT MODE COMMAND
void mode(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients){

    //PARSIN WITH SS
    std::vector<std::string> args = parser(command, ' ');
    int argc = args.size();

    std::cout << "full cmd : <"<< trim(command, "\r\n") << "> "<< std::endl;
    
    for (int i = 0; i < args.size(); i++){
        std::cout << "cmd      : <" << args[i] << "> " << std::endl;
    }
    std::cout << std::endl;

    std::string channel_name;
    std::string mode;
    std::string last_param;    

// // START
    if (argc == 1)
        mode_one_param();
    else if (argc == 2){
        channel_name = args[1];
        mode_two_params(channels, client, channel_name);
    }
    else if (argc == 3){
        channel_name = args[1];
        mode         = args[2];
        mode_three_params(channels, client, channel_name, mode);
    }
    else if (argc == 4){
        channel_name = args[1];
        mode         = args[2];      
        last_param   = args[3];      
        mode_four_params(channels, client, channel_name, mode, last_param);
    }
    else{
        // sendMessage(client.get_fd(), ERR_UNKNOWNMODE());
        std::cout << "ERR_UNKNOWNMODE" << std::endl;
    }
    std::cout << "cmd      : <" << args[0] << "> " << std::endl;
    std::cout << "channel  : <" << channel_name << "> " << std::endl;
    std::cout << "mode     : <" << mode << "> " << std::endl;
    std::cout << "params   : <" << last_param << "> " << std::endl;
};
