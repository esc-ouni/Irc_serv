#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

void play(std::string command, Client &client){
    std::vector<std::string> args = parser(command, ' ');
    std::string play = "/usr/bin/afplay b_tmp/";

    int argc = args.size();

    if (argc == 2){
        if (args[0] == "PLAY" && args[1] == "sba7_lkheir"){
                play += "m.mp3 &";
                send_message(client.get_fd(), RPL("Now Playing sba7_lkheir ...", client.get_nickname()));
                std::system(play.c_str());
                return ;
        }
        else
            send_message(client.get_fd(), RPL("PLAY <song_title>", client.get_nickname()));
    }
    else
        send_message(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "PLAY"));
};