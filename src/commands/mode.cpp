/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 14:17:20 by idouni            #+#    #+#             */
/*   Updated: 2023/12/05 18:46:15 by idouni           ###   ########.fr       */
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


bool    valid_option(std::string &option){
    if (option.length() != 2 || option.empty() || option.at(0) != '+' || option.at(0) != '-'){
        if (option.at(1) != 'i' || option.at(1) != 't' || option.at(1) != 'k' || option.at(1) != 'o' || option.at(1) != 'l')
            return (false);        
    }
    return (true);
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

//IMPLEMENT MODE COMMAND
void mode(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients){

    //PARSIN WITH SS
    std::vector<std::string> args = parser(command, ' ');
    int argc = args.size();

//     std::cout << "full cmd : <"<< trim(command, "\r\n") << "> "<< std::endl;
    
//     for (int i = 0; i < args.size(); i++){
//         std::cout << "cmd      : <" << args[i] << "> " << std::endl;
//     }


// // START
    if (argc == 1)
        // Reply(650): MODE <target> [[(+|-)]<modes> [<mode-parameters>]]
    if (argc == 2){
        if (!channel_exist(channels, args[1])){
            //  ERR_NOSUCHCHANNEL (403)
            return ;   
        }
        else{
            //  RPL_CHANNELMODEIS (324)
            //  RPL_CREATIONTIME (329) 
            std::cout << "SHOW AVAILABLE MODS ON THIS CHANNEL !" << std::endl;
            return ;
        }  
    }
    if (argc == 3){
        if (!channel_exist(channels, args[1])){
            //  ERR_NOSUCHCHANNEL (403)
            return ;   
        }
        if (!client.is_operator(channels[args[1]])){
            //  ERR_CHANOPRIVSNEEDED (482)
            return ;
        }    
        if (!valid_option(args[2])){
            // ERR_UMODEUNKNOWNFLAG (501)
            return ;
        }
        if (args[2].at(1) == 'k' || args[2].at(1) == 'o'){
            // ERR_INVALIDMODEPARAM (696)
            return ;
        }
        if (args[2].at(1) == 'i'){
            if (args[2].at(0) == '+' && !channels[args[1]].get_option_i()){
                // broadcast the mode change to all channel users
                // RP => :UserNick!UserHost MODE #Channel +i
                // channels[args[1]].broadcast_message();
                channels[args[1]].set_option_i(true);
                return ;
            }
            if (args[2].at(0) == '-' && channels[args[1]].get_option_i()){
                // broadcast the mode change to all channel users
                // RP => :UserNick!UserHost MODE #Channel -i
                // channels[args[1]].broadcast_message();
                channels[args[1]].set_option_i(false);
                return ;
            }
            return ;
        }
        else if (args[2].at(1) == 't'){
            if (args[2].at(0) == '+' && !channels[args[1]].get_option_t()){
                // broadcast the mode change to all channel users
                // RP => :UserNick!UserHost MODE #Channel +t
                // channels[args[1]].broadcast_message();
                channels[args[1]].set_option_t(true);
                return ;
            }
            if (args[2].at(0) == '-' && channels[args[1]].get_option_t()){
                // broadcast the mode change to all channel users
                // RP => :UserNick!UserHost MODE #Channel -t
                // channels[args[1]].broadcast_message();
                channels[args[1]].set_option_t(false);
                return ;
            }
            return ;
        }
        if (args[2].at(0) == '-' && channels[args[1]].get_option_l()){
            if (contains_only_nums(args[3]) != -1){
                if (channels[args[1]].set_limit(CHANNEL_LIMIT)){
                    // :UserNick!UserHost MODE #Channel -l
                    channels[args[1]].set_option_l(false); 
                    return ;
                }
            }
            return ;
        }
    }
    if (argc == 4){
        if (!channel_exist(channels, args[1])){
            //  ERR_NOSUCHCHANNEL (403)
            return ;   
        }
        if (!client.is_operator(channels[args[1]])){
            //  ERR_CHANOPRIVSNEEDED (482)
            return ;
        }    
        if (!valid_option(args[2])){
            // ERR_UMODEUNKNOWNFLAG (501)
            return ;
        }
        if (args[2].at(1) != 'i' || args[2].at(1) != 't'){
            // ERR_INVALIDMODEPARAM (696)
            return ;
        }
        if (args[2].at(1) == 'k'){
            if (args[2].at(0) == '+' && !channels[args[1]].get_option_k()){
                if (!is_valid_password(args[3])){
                    // ERR_INVALIDKEY (525)
                    return ;
                }
                if (channels[args[1]].set_password(args[3])){
                    // broadcast the mode change to all channel users
                    // :UserNick!UserHost MODE #Channel +k key
                    // channels[args[1]].broadcast_message();
                    channels[args[1]].set_option_k(true);                
                }
                return ;
            }
            if (args[2].at(0) == '-' && channels[args[1]].get_option_k()){
                if (args[3] == channels[args[1]].get_password()){
                    // broadcast the mode change to all channel users
                    // :UserNick!UserHost MODE #Channel -k key
                    // channels[args[1]].broadcast_message();
                    channels[args[1]].set_option_k(false);
                }
                return ;
            }
            return ;
        }
        else if (args[2].at(1) == 'l'){
            if (args[2].at(0) == '+' && !channels[args[1]].get_option_l()){
                if (contains_only_nums(args[3]) != -1){
                    if (channels[args[1]].set_limit(contains_only_nums(args[3]))){
                        // :UserNick!UserHost MODE #Channel +l number
                        channels[args[1]].set_option_l(true);
                    }
                }
                return ;
            }
            return ;
        }
        
    }
    // ERR_UNKNOWNMODE (472)
};
        // else if (args[2].at(1) == 'o'){
            // TBC
        //     return ;
        // }
    // std::cout << "cmd      : <" << args[0] << "> " << std::endl;
    // std::cout << "channel  : <" << args[1] << "> " << std::endl;
    // std::cout << "mode     : <" << args[2] << "> " << std::endl;
    // std::cout << "params   : <" << args[3] << "> " << std::endl;