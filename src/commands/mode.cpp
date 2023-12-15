/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 14:17:20 by idouni            #+#    #+#             */
/*   Updated: 2023/12/15 16:19:29 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

void mode(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients){
    std::vector<std::string> args = parser(command, ' ');
    if (args.at(0) != "MODE")
        return ;
    int argc = args.size();

    std::string channel_name;
    std::string mode;
    std::string last_param;   
    
    if (argc == 1)
        mode_one_param(client);
    else if (argc == 2){
        channel_name   = args[1];
        mode_two_params(channels, client, channel_name);
    }
    else if (argc == 3 || argc == 4){
        channel_name   = args[1];
        mode           = args[2];
        if (argc == 4)
            last_param = args[3];
        mode_with_params(channels, client, channel_name, mode, clients, last_param);
    }
    else{
        send_message(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "MODE"));
    }
};

void mode_one_param(Client &excuter){
    send_message(excuter.get_fd(), RPL("MODE <target> [[(+|-)]<modes> [<mode-parameters>]]", excuter.get_nickname()));
};

void send_mode_info(Client &excuter, Channel &channel){
    if (!channel.show_mode().empty()){
        send_message(excuter.get_fd(), RPL_CHANNELMODEIS(excuter.get_nickname(), channel.get_name(), channel.show_mode()));
    }
    send_message(excuter.get_fd(), RPL_CREATIONTIME(excuter.get_nickname(), channel.get_name(), channel.get_creation_date()));
};

void mode_two_params(std::map<std::string, Channel>& channels, Client &excuter, std::string &channel_name){
    if (!channel_exist(channels, channel_name)){
        send_message(excuter.get_fd(), ERR_NOSUCHCHANNEL(excuter.get_nickname(), channel_name));
        return ;
    }
    else if (!channels[channel_name].is_member(excuter)){
        send_message(excuter.get_fd(), ERR_NOTONCHANNEL(excuter.get_nickname(), channel_name));
        return ;   
    }
    else{
        send_mode_info(excuter, channels[channel_name]);
        return ;
    }
};

void mode_with_params(std::map<std::string, Channel>& channels, Client &excuter, std::string &channel_name, std::string &mode, std::map<int, Client> &clients, std::string &last_param){
    std::vector<std::string> param;
    std::string              s_mode;
    std::string              s_param;
    size_t                   param_c = 0;
    
    if (!channel_exist(channels, channel_name)){
        send_message(excuter.get_fd(), ERR_NOSUCHCHANNEL(excuter.get_nickname(), channel_name));
        return ;   
    }
    if (!excuter.is_operator(channels[channel_name]) && !excuter.is_IRC_op()){
        send_message(excuter.get_fd(), ERR_CHANOPRIVSNEEDED(excuter.get_nickname(), channel_name));
        return ;
    }
    if (!valid_full_option(mode)){
        send_message(excuter.get_fd(), ERR_INVALIDMODEPARAM(excuter.get_nickname(), channel_name, "MODE"));
        return ;
    }

    param  = mode_parser(last_param, ',');
    param_c = param.size();
    
    for (size_t i = 1; i < mode.size(); i++){
        if (i <= param_c)
            s_param = param.at(i - 1);
        else
            s_param = "";
        s_mode += mode.at(0);
        s_mode += mode.at(i);
        if (!valid_option(s_mode)){
            send_message(excuter.get_fd(), ERR_UMODEUNKNOWNFLAG(excuter.get_nickname(), channel_name, s_mode.at(1)));
        }
        else
            execute_mode(channels, excuter, channel_name, s_mode, clients, s_param);
        s_mode.clear();
        s_param.clear();
    }
};

void mode_flag_i(std::map<std::string, Channel>& channels, Client &excuter, std::string &channel_name, std::string &mode){
    if (mode.at(0) == '+' && !channels[channel_name].get_option_i()){
        channels[channel_name].broadcast_message(MODE_CHANGED(excuter.get_nickname(), excuter.get_client_host(), channel_name, "+i", ""));
        channels[channel_name].set_option_i(true);
        return ;
    }
    if (mode.at(0) == '-' && channels[channel_name].get_option_i()){
        channels[channel_name].broadcast_message(MODE_CHANGED(excuter.get_nickname(), excuter.get_client_host(), channel_name, "-i", ""));
        channels[channel_name].set_option_i(false);
        return ;
    }  
    return ;
};

void mode_flag_t(std::map<std::string, Channel>& channels, Client &excuter, std::string &channel_name, std::string &mode){
    if (mode.at(0) == '+' && !channels[channel_name].get_option_t()){
        channels[channel_name].broadcast_message(MODE_CHANGED(excuter.get_nickname(), excuter.get_client_host(), channel_name, "+t", ""));
        channels[channel_name].set_option_t(true);
        return ;
    }
    else if (mode.at(0) == '-' && channels[channel_name].get_option_t()){
        channels[channel_name].broadcast_message(MODE_CHANGED(excuter.get_nickname(), excuter.get_client_host(), channel_name, "-t", ""));
        channels[channel_name].set_option_t(false);
        return ;
    }
    return ;
};

void mode_flag_l(std::map<std::string, Channel>& channels, Client &excuter, std::string &channel_name, std::string &mode, std::string &last_param){
    if (mode.at(0) == '+' && contains_only_nums(last_param) != channels[channel_name].get_limit()){
        if (contains_only_nums(last_param) != -1){
            if (channels[channel_name].set_limit(contains_only_nums(last_param))){
                channels[channel_name].broadcast_message(MODE_CHANGED(excuter.get_nickname(), excuter.get_client_host(), channel_name, "+l", last_param));
                channels[channel_name].set_option_l(true);
            }
        }
        return ;
    }
    if (mode.at(0) == '-' && channels[channel_name].get_option_l()){
        channels[channel_name].set_limit(CHANNEL_LIMIT);
        channels[channel_name].broadcast_message(MODE_CHANGED(excuter.get_nickname(), excuter.get_client_host(), channel_name, "-l", ""));
        channels[channel_name].set_option_l(false);
        return ;
    }
    return ;
};

void mode_flag_k(std::map<std::string, Channel>& channels, Client &excuter, std::string &channel_name, std::string &mode, std::string &last_param){
    if (mode.at(0) == '+' && !channels[channel_name].get_option_k()){
        if (!is_valid_password(last_param)){
            send_message(excuter.get_fd(), ERR_PASSINVALID(excuter.get_nickname()));
            return ;
        }
        if (channels[channel_name].set_password(last_param)){
            channels[channel_name].broadcast_message(MODE_CHANGED(excuter.get_nickname(), excuter.get_client_host(), channel_name, "+k", ""));
            channels[channel_name].set_option_k(true);                
            channels[channel_name].lock();
        }
        return ;
    }
    else if (mode.at(0) == '-' && channels[channel_name].get_option_k()){
        if (last_param == channels[channel_name].get_password()){
            channels[channel_name].broadcast_message(MODE_CHANGED(excuter.get_nickname(), excuter.get_client_host(), channel_name, "-k", ""));
            channels[channel_name].set_option_k(false);
            channels[channel_name].unlock();
        }
        else
            send_message(excuter.get_fd(), ERR_PASSWDMISMATCH(excuter.get_nickname()));
        return ;
    }
    return ;
};

void mode_flag_o(std::map<std::string, Channel>& channels, Client &excuter, std::string &channel_name, std::string &mode, std::map<int, Client> &clients, std::string &last_param){
    if (!channels[channel_name].is_member(last_param)){
        if (!last_param.empty())
            send_message(excuter.get_fd(), ERR_USERNOTINCHANNEL(excuter.get_nickname(), last_param, channel_name));
        return ;
    }
    if (mode.at(0) == '-' && channels[channel_name].is_operator(last_param)){
        channels[channel_name].broadcast_message(MODE_CHANGED(excuter.get_nickname(), excuter.get_client_host(), channel_name, "-o", last_param));
        channels[channel_name].unpromote(channels[channel_name].is_member(last_param));         
        return ;
    }
    if (mode.at(0) == '+' && !channels[channel_name].is_operator(last_param)){
        channels[channel_name].broadcast_message(MODE_CHANGED(excuter.get_nickname(), excuter.get_client_host(), channel_name, "+o", last_param));
        channels[channel_name].promote(clients[channels[channel_name].is_member(last_param)]);         
        return ;
    }
    return ;
};

void execute_mode(std::map<std::string, Channel>& channels, Client &excuter, std::string &channel_name, std::string &mode, std::map<int, Client> &clients, std::string &last_param){

    if (mode.at(1) == 'i'){
        mode_flag_i(channels, excuter, channel_name, mode);
        return ;
    }
    if (mode.at(1) == 't'){
        mode_flag_t(channels, excuter, channel_name, mode);
        return ;
    }
    if (mode.at(1) == 'l'){
        mode_flag_l(channels, excuter, channel_name, mode, last_param);
        return ;
    }
    if (mode.at(1) == 'k'){
        mode_flag_k(channels, excuter, channel_name, mode, last_param);
        return ;
    }
    if (mode.at(1) == 'o'){
        mode_flag_o(channels, excuter, channel_name, mode, clients, last_param);
        return ;
    }
    return ;
};
