#include "../headers/Irc.hpp"
#include "../headers/Channel.hpp"
#include "../headers/commands.hpp"

void send_message(int client_socket, std::string message) {
    if (send(client_socket, message.c_str(), message.length(), 0) == -1)
        std::cerr << "Err: failling sending message to the client !" << std::endl;
    usleep(300);
};


int main() {
    // int             client_socket;
    // struct sockaddr address;

    // address.

    // client_socket = socket(AF_LOCAL ,PF_INET, SOCK_DGRAM);

    // if (connect(client_socket, &address, sizeof(address)) == -1){
    //     std::cerr << "Err: failed connecting the socket ..." << std::endl;
    //     return ;
    // }


    // Connect to the IRC server

    send_message(client_socket, "PASS pass\r\n");
    send_message(client_socket, "NICK MusicBot\r\n");
    send_message(client_socket, "USER 1 0 *\r\n");
    // Join a channel
    send_message(client_socket, "JOIN #Music\r\n");

    //send what it receves in PRRIVMSG to #Music

    close(client_socket);
    return 0;
}


