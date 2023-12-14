/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 19:07:04 by idouni            #+#    #+#             */
/*   Updated: 2023/12/14 17:59:17 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Irc.hpp"
#include "../headers/Channel.hpp"
#include "../headers/commands.hpp"
#include <netdb.h>

std::string trim(std::string &str, std::string charstotrim) {
    size_t endpos = str.find_last_not_of(charstotrim);

    if (std::string::npos != endpos) {
        str = str.substr(0, endpos + 1);
    }
    return (str);
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

long int contains_only_nums(std::string string){
    if (string.empty())
        return (-1);
    for (size_t i = 0; i < string.length(); i++){
        if (string.c_str()[0] == '+')
            continue ;
        if (!isdigit(string.c_str()[i]) || i == 10)
            return (0);
    }
    return (std::atol(string.c_str()));
};

void send_message(int client_socket, std::string message) {
    if (send(client_socket, message.c_str(), message.length(), 0) == -1)
        std::cerr << "Err: failling sending message to the client !" << std::endl;
    usleep(300);
};


int main(int argc, char *argv[]) {
    if (argc != 3){
        std::cerr << "Usage: <Ip> <port>!" << std::endl;
        return 0;    
    }
    struct sockaddr_in addr;
    std::string message;

    std::vector<std::string> args;

    struct pollfd pfd;

    std::string play;
    char        buff[512];


    bzero(&addr, sizeof(addr));

    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port        = htons(contains_only_nums(argv[2]));

    int socket_end = socket(PF_INET, SOCK_STREAM, getprotobyname("tcp")->p_proto);

    if (connect(socket_end, (struct sockaddr *)&addr, sizeof(addr)) == -1){
        std::cerr << "Err: failling to connect socket!" << std::endl;
        return 0;
    }
    std::cout << "SOCKET CONNECTED SUCCESSFULLY !" << std::endl;
    
    pfd.fd     = socket_end;
    pfd.events = POLLIN;

    int ready_fd;

    int read_bytes = 0;
    send_message(socket_end, "PASS pass\r\n");
    send_message(socket_end, "NICK Emet\r\n");
    send_message(socket_end, "USER 1 0 *\r\n");

    while (true){
        ready_fd = poll(&pfd, 1, -1);
        if (pfd.revents == POLLIN){
            read_bytes = recv(socket_end, buff, 512, 0);
            if (read_bytes > 0){
                std::cout << "PING" << std::endl;
                buff[read_bytes] = '\0';
                // std::cout << buff << std::endl;
                play = buff;
                if (play.find("MSG_TO_SD") != std::string::npos){
                    play.erase(0, play.find("MSG_TO_SD"));
                    args = parser(play, ' ');
                    
                    if (args.size() >= 1){
                        message = "PRIVMSG " + args[1] + " :You have been pinged, you are to noisy a zamel \r\n";
                        send_message(socket_end, message);
                    }
                }
            }
            else if (!read_bytes){
                std::cout << "SOCKET CLOSED !" << std::endl; 
                break ;
            }
            else if (read_bytes == -1){
                std::cout << "RECV FAILED !" << std::endl; 
                break ; 
            }
        }
        else if (pfd.revents == 17){
            std::cout << "CONNECTION CLOSED !" << std::endl; 
            break;
        }
        else if (pfd.revents == -1){
            std::cout << "POLL FAILED !" << std::endl; 
            break;
        }
        bzero(buff, sizeof(buff));
        play.clear();
    }

    return (0);
}