/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MusicBot.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 19:07:04 by idouni            #+#    #+#             */
/*   Updated: 2023/12/13 19:31:39 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Irc.hpp"
#include "../headers/Channel.hpp"
#include "../headers/commands.hpp"
#include <netdb.h>

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
    send_message(socket_end, "NICK MusicBot\r\n");
    send_message(socket_end, "USER 1 0 *\r\n");
    send_message(socket_end, "JOIN #music\r\n");
    send_message(socket_end, "TOPIC #music :Use <!Bot 'dance'> Command For The Music Of The Day\r\n");

    while (true){
        ready_fd = poll(&pfd, 1, -1);
        if (pfd.revents == POLLIN){
            read_bytes = recv(socket_end, buff, 512, 0);
            if (read_bytes > 0){
                // std::cout << "PING" << std::endl;
                buff[read_bytes] = '\0';
                // std::cout << buff << std::endl;
                play = buff;
                if (play.find("!Bot dance") != std::string::npos){
                    play = "/usr/bin/afplay b_tmp/m.mp3 &";
                    std::system(play.c_str());
                    send_message(socket_end, "PRIVMSG #music : Now playing sba7 lkheir \r\n");
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














































