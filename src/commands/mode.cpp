/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 14:17:20 by idouni            #+#    #+#             */
/*   Updated: 2023/12/07 17:54:54 by idouni           ###   ########.fr       */
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
        args.push_back(trim(token, "\r\n"));
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

void mode_one_param(Client &excuter){
    std::cout << "Reply(650): MODE <target> [[(+|-)]<modes> [<mode-parameters>]]" << std::endl;
    // Reply(650): MODE <target> [[(+|-)]<modes> [<mode-parameters>]]
    send_message(excuter.get_fd(), RPL("MODE <target> [[(+|-)]<modes> [<mode-parameters>]]", excuter.get_nickname()));
};

void send_mode_info(Client &excuter, Channel &channel){
  std::string notice;
        if (!channel.show_mode().empty()){
            notice = RPL_CHANNELMODEIS(excuter.get_nickname(), channel.get_name(), channel.show_mode());
            send_message(excuter.get_fd(), notice);
        }
        notice = RPL_CREATIONTIME(excuter.get_nickname(), channel.get_name(), channel.get_creation_date());
        send_message(excuter.get_fd(), notice);
        std::cout << "SHOW AVAILABLE MODS ON THIS CHANNEL !" << std::endl;
        std::cout << "CHANNEL mode : " << channel.show_mode() << std::endl;
};


void mode_two_params(std::map<std::string, Channel>& channels, Client &excuter, std::string &channel_name){
  std::string notice;
    if (!channel_exist(channels, channel_name)){
        std::cout << "ERR_NOSUCHCHANNEL" << std::endl;
        send_message(excuter.get_fd(), ERR_NOSUCHCHANNEL(excuter.get_nickname(), channel_name));
        return ;
    }
    else if (!channels[channel_name].is_member(excuter)){
        std::cout << "ERR_NOTAMEMBER" << std::endl;
        send_message(excuter.get_fd(), ERR_NOTONCHANNEL(excuter.get_nickname(), channel_name));
        return ;   
    }
    else{
        send_mode_info(excuter, channels[channel_name]);
        return ;
    }
};

void mode_three_params(std::map<std::string, Channel>& channels, Client &excuter, std::string &channel_name, std::string &mode){
  std::string notice;
    if (!channel_exist(channels, channel_name)){
        notice = ERR_NOSUCHCHANNEL(excuter.get_nickname(), channel_name);
        send_message(excuter.get_fd(), notice);
        std::cout << "ERR_NOSUCHCHANNEL" << std::endl;
        return ;   
    }
    if (!excuter.is_operator(channels[channel_name])){
        send_message(excuter.get_fd(), ERR_CHANOPRIVSNEEDED(excuter.get_nickname(), channel_name));
        std::cout << "ERR_CHANOPRIVSNEEDED" << std::endl;
        return ;
    }    
    if (!valid_option(mode)){
        send_message(excuter.get_fd(), ERR_UMODEUNKNOWNFLAG(excuter.get_nickname(), channel_name));
        std::cout << "ERR_UMODEUNKNOWNFLAG" << std::endl;
        return ;
    }
    if (mode.at(1) == 'k' || mode.at(1) == 'o' || (mode.at(1) == 'l' && mode.at(0) == '+')){
        send_message(excuter.get_fd(), ERR_INVALIDMODEPARAM(excuter.get_nickname(), channel_name, mode));
        std::cout << "ERR_INVALIDMODEPARAM" << std::endl;
        return ;
    }
    if (mode.at(1) == 'i'){
        if (mode.at(0) == '+' && !channels[channel_name].get_option_i()){
            std::cout << "MODE #Channel +i" << std::endl;
            channels[channel_name].broadcast_message(MODE_CHANGED(excuter.get_nickname(), channel_name, "+i", ""));
            channels[channel_name].set_option_i(true);
            return ;
        }
        if (mode.at(0) == '-' && channels[channel_name].get_option_i()){
            std::cout << "MODE #Channel -i" << std::endl;
            channels[channel_name].broadcast_message(MODE_CHANGED(excuter.get_nickname(), channel_name, "-i", ""));
            channels[channel_name].set_option_i(false);
            return ;
        }
        return ;
    }
    else if (mode.at(1) == 't'){
        if (mode.at(0) == '+' && !channels[channel_name].get_option_t()){
            std::cout << "MODE #Channel +t" << std::endl;
            channels[channel_name].broadcast_message(MODE_CHANGED(excuter.get_nickname(), channel_name, "+t", ""));
            channels[channel_name].set_option_t(true);
            return ;
        }
        else if (mode.at(0) == '-' && channels[channel_name].get_option_t()){
            std::cout << "MODE #Channel -t" << std::endl;
            channels[channel_name].broadcast_message(MODE_CHANGED(excuter.get_nickname(), channel_name, "-t", ""));
            channels[channel_name].set_option_t(false);
            return ;
        }
        return ;
    }
    if (mode.at(1) == 'l' && mode.at(0) == '-' && channels[channel_name].get_option_l()){
        channels[channel_name].set_limit(CHANNEL_LIMIT);
        std::cout << "MODE #Channel -l" << std::endl;
        channels[channel_name].broadcast_message(MODE_CHANGED(excuter.get_nickname(), channel_name, "-l", ""));
        channels[channel_name].set_option_l(false);
        return ;
    }
};

void mode_four_params(std::map<std::string, Channel>& channels, Client &excuter, std::string &channel_name, std::string &mode, std::string &last_param){
  std::string notice;
    if (!channel_exist(channels, channel_name)){
            std::cout << "ERR_NOSUCHCHANNEL" << std::endl;
            send_message(excuter.get_fd(), ERR_NOSUCHCHANNEL(excuter.get_nickname(), channel_name));
        return ;   
    }
    if (!excuter.is_operator(channels[channel_name])){
        send_message(excuter.get_fd(), ERR_CHANOPRIVSNEEDED(excuter.get_nickname(), channel_name));
        std::cout << "ERR_CHANOPRIVSNEEDED" << std::endl;
        return ;
    }    
    if (!valid_option(mode)){
        send_message(excuter.get_fd(), ERR_UMODEUNKNOWNFLAG(excuter.get_nickname(), channel_name));
        std::cout << "ERR_UMODEUNKNOWNFLAG" << std::endl;
        return ;
    }
    if (mode.at(1) == 'i' || mode.at(1) == 't'){
        send_message(excuter.get_fd(), ERR_INVALIDMODEPARAM(excuter.get_nickname(), channel_name, mode));
        std::cout << "ERR_INVALIDMODEPARAM" << std::endl;
        return ;
    }
    if (mode.at(1) == 'k'){
        if (mode.at(0) == '+' && !channels[channel_name].get_option_k()){
            if (!is_valid_password(last_param)){
                send_message(excuter.get_fd(), ERR_INVALIDKEY(excuter.get_nickname(), channel_name));
                std::cout << "ERR_INVALIDKEY" << std::endl;
                return ;
            }
            if (channels[channel_name].set_password(last_param)){
                std::cout << "MODE #Channel +k key" << std::endl;
                channels[channel_name].broadcast_message(MODE_CHANGED(excuter.get_nickname(), channel_name, "+k", ""));
                channels[channel_name].set_option_k(true);                
                channels[channel_name].lock();
            }
            return ;
        }
        else if (mode.at(0) == '-' && channels[channel_name].get_option_k()){
            if (last_param == channels[channel_name].get_password()){
                std::cout << "MODE #Channel -k key" << std::endl;
                channels[channel_name].broadcast_message(MODE_CHANGED(excuter.get_nickname(), channel_name, "-k", ""));
                channels[channel_name].set_option_k(false);
                channels[channel_name].unlock();
            }
            return ;
        }
        return ;
    }
    else if (mode.at(1) == 'l'){
        if (mode.at(0) == '+' && contains_only_nums(last_param) != channels[channel_name].get_limit()){
            if (contains_only_nums(last_param) != -1){
                if (channels[channel_name].set_limit(contains_only_nums(last_param))){
                    std::cout << "MODE #Channel +l number" << std::endl;
                    channels[channel_name].broadcast_message(MODE_CHANGED(excuter.get_nickname(), channel_name, "+l", last_param));
                    channels[channel_name].set_option_l(true);
                }
            }
        }
        return ;
    }        
};



//IMPLEMENT MODE COMMAND
void mode(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients){
    std::vector<std::string> args = parser(command, ' ');
    if (args.at(0) != "MODE")
        return ;
    int argc = args.size();

    std::string channel_name;
    std::string mode;
    std::string last_param;    

    // std::cout << "size     : <" << argc << "> " << std::endl;
    // std::cout << "cmd      : <" << args[0] << "> " << std::endl;
    // std::cout << "channel  : <" << channel_name << "> " << std::endl;
    // std::cout << "mode     : <" << mode << "> " << std::endl;
    // std::cout << "params   : <" << last_param << "> " << std::endl;
    
    if (argc == 1)
        mode_one_param(client);
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
        send_message(client.get_fd(), ERR_UNKNOWNMODE(client.get_nickname(), channel_name, mode));
        std::cout << "ERR_UNKNOWNMODE" << std::endl;
    }
};
