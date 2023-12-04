#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

//IMPLEMENT MODE COMMAND
void mode(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients){
    std::cout << "full cmd : <"<< trim(command, "\r\n") << "> "<< std::endl;
    std::cout << "channel  : <" << extract_channel_name(command) << "> " << std::endl;
    std::cout << "mode     : <" << extract_target(command) << "> " << std::endl;
    std::cout << "params   : <" << extract_reason(command) << "> " << std::endl;
};