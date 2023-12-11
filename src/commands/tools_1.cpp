#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

std::string extract_channel_name(std::string& command) {
    size_t n = command.find('#');
    std::string ch_name = "";
    
    if (n != std::string::npos) {
        ch_name = command.substr(n);
        n = ch_name.find(' ');
        if (n != std::string::npos){
            ch_name = ch_name.substr(0, n);
        }
    }
    return (trim(ch_name, "\r\n"));
};

bool channel_exist(std::map<std::string, Channel>& channels, std::string &needle){
    std::map<std::string, Channel>::iterator it = channels.begin();
    
    while (it != channels.end()){
        if (needle == it->second.get_name())
            return (true);
        it++;
    }
    return (false);
};

std::string extract_topic(std::string& command) {
    std::string topic = "";
    size_t n = command.find('#');    
    
    if (n != std::string::npos) {
        n = command.find(' ', n);
        if (n != std::string::npos) {
            topic = command.substr(n + 1);
            if (!topic.empty() && !topic.compare(0, 1, ":")){
                topic.erase(0, 1);
            }
        }
    }
    return (trim(topic, "\r\n"));
}

std::string extract_target(std::string& command) {
    std::string target = "";
    size_t n2 = 0, n = command.find('#');    
    
    if (n != std::string::npos) {
        n = command.find(' ', n);
        if (n != std::string::npos) {
            n2 = command.find(' ', n + 1);
            if (n2 != std::string::npos) {
                target = command.substr(n + 1, (n2 - n - 1));
                if (!target.empty() && !target.compare(0, 1, ":")){
                    target.erase(0, 1);
                }
            }
        }
    }
    return (trim(target, "\r\n"));
};

std::string extract_reason(std::string& command) {
    std::string reason = "";
    size_t      n = command.find('#');    
    
    if (n != std::string::npos) {
        
        n = command.find(' ', n);
        if (n != std::string::npos) {
            n = command.find(' ', n + 1);
            if (n != std::string::npos) {
                reason = command.substr(n + 1);
                if (!reason.empty() && !reason.compare(0, 1, ":")){
                    reason.erase(0, 1);
                }
            }
        }
    }
    return (trim(reason, "\r\n"));
};

std::vector<std::string> mode_parser(std::string &full_command, char dilimeter){
    std::vector<std::string> args;
    std::stringstream        stream(trim(full_command, "\r\n"));
    std::string              token;
    
    if (full_command.empty())
        return args;
    while (!std::getline(stream , token,  dilimeter).eof()){
        args.push_back(token);
    }
    if (!token.empty()){
        args.push_back(token);
    }
    return args;
};