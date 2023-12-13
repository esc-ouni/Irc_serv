
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <fcntl.h> // for fcntl
#include <poll.h>  // for poll


#include "../headers/Irc.hpp"
#include "../headers/Channel.hpp"
#include "../headers/commands.hpp"

void send_message(int client_socket, std::string message) {
    if (send(client_socket, message.c_str(), message.length(), 0) == -1)
        std::cerr << "Err: failling sending message to the client !" << std::endl;
    usleep(100);
};


const char* server_ip = "127.0.0.1"; // Replace with your IRC server's IP address
const int server_port = 6667;        // Replace with your IRC server's port


int main() {
        std::string play;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Set socket to non-blocking mode
    // int flags = fcntl(sock, F_GETFL, 0);
    // if (flags == -1) {
    //     perror("fcntl failed to get flags");
    //     close(sock);
    //     return -1;
    // }
    // if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1) {
    //     perror("fcntl failed to set non-blocking flag");
    //     close(sock);
    //     return -1;
    // }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    // Connect to the IRC server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        if (errno != EINPROGRESS) {
            perror("Connect failed");
            close(sock);
            return -1;
        }
    }

    // IRC protocol handshake
    send_message(sock, "PASS pass\r\n");
    send_message(sock, "NICK MusicBot\r\n");
    send_message(sock, "USER 1 0 * \r\n");

    // Join a channel
    send_message(sock, "JOIN #music\r\n");

    // Main loop to listen to messages from the server and respond
    char buffer[512];
    struct pollfd pfd = {sock, POLLIN, 0};
    while (true) {
        int poll_result = poll(&pfd, 1, -1); // Infinite timeout
        if (poll_result == -1) {
            perror("poll failed");
            break;
        }
        if (poll_result > 0) {
            ssize_t bytes_received = recv(sock, buffer, sizeof(buffer), 0);
            if (bytes_received < 0) {
                perror("Receive failed");
                break;
            }
            buffer[bytes_received] = '\0';
            std::cout << buffer << std::endl;

            play = buffer;
            if (play.find("!Bot dance") != std::string::npos){
                play = "/usr/bin/afplay b_tmp/m.mp3 &";
                std::system(play.c_str());
                send_message(sock, "PRIVMSG #music : Now playing sba7 lkheir \r\n");
            }
            play.clear();
            bzero(buffer, sizeof(buffer));
        }
    }

    close(sock);
    return 0;
}
