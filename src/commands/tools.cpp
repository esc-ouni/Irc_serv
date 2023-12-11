#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

bool channel_name_is_valid(std::string &channel_name){
    if (channel_name.empty() || (channel_name.length() > 50))
        return(false);
    if (channel_name.c_str()[0] != '#')
        return(false);
    for (int i = 1; i < channel_name.length(); i++){
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

std::string trim(std::string &str, std::string charstotrim) {
    size_t endpos = str.find_last_not_of(charstotrim);

    if (std::string::npos != endpos) {
        str = str.substr(0, endpos + 1);
    }
    return (str);
};


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
    std::stringstream        stream(trim(full_command, "\r\n"));
    std::string              token;
    
    if (full_command.empty())
        return args;
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
    if (option.length() != 2)
        return (false);        
    if ((option.at(0) != '+' && option.at(0) != '-'))
        return (false);
    for (int i = 0; i < option.size(); i++){
        if (std::isalpha(option.at(i)))
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