/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 17:34:31 by idouni            #+#    #+#             */
/*   Updated: 2023/12/20 11:30:12 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

bool channel_name_is_valid(std::string &channel_name){
    if (channel_name.empty() || (channel_name.length() > 50))
        return(false);
    if (channel_name.c_str()[0] != '#')
        return(false);
    for (size_t i = 1; i < channel_name.length(); i++){
        if ((!std::isalnum(channel_name.c_str()[i]) && !valid_sp_character(channel_name.c_str()[i])) || std::isspace(channel_name.c_str()[i]))
            return(false);
    }
    return (true);
}

bool valid_sp_character(int c){
    if (c == '_' || c == '-' || c == '.')
        return (true);
    return (false);
}

void send_message(int clientSocket, std::string message) {
    if (send(clientSocket, message.c_str(), message.length(), 0) == -1)
        std::cerr << "Err: failling sending message to the client !" << std::endl;
};

long int contains_only_nums(std::string string){
    if (string.empty())
        return (-1);
    for (size_t i = 0; i < string.length(); i++){
        if (string.c_str()[0] == '+')
            continue ;
        if (!isdigit(string.c_str()[i]) || i == 8)
            return (-1);
    }
    return (std::atol(string.c_str()));
};

std::string trim(std::string &str) {
    int l = str.length() - 1;
    
    while (!str.empty() && (str.at(l) == '\r' || str.at(l) == '\n')) {
        str.erase(str.begin()+l);
        l--;
    }
    return (str);
};

std::vector<std::string> parser(std::string &full_command, char dilimeter){
    std::vector<std::string> args;
    std::stringstream        stream(trim(full_command));
    std::string              token;
    
    if (full_command.empty())
        return (args);
    while (!std::getline(stream , token,  dilimeter).eof()){
        if (!token.empty())
            args.push_back(token);
    }
    if (!token.empty()){
        args.push_back(token);
    }
    return args;
};

bool valid_full_option(std::string &option){   
    if ((option.at(0) != '+' && option.at(0) != '-'))
        return (false);
    for (size_t i = 1; i < option.size(); i++){
        if (!std::isalpha(option.at(i)))
            return (false);    
    }
    return (true);    
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

std::string time_to_string(time_t timeVal) {
    std::stringstream convert;
    convert << timeVal;
    return (convert.str());
};