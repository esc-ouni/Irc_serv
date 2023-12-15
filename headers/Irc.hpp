
#pragma once

/* includes */
#include <ctime>
#include <sys/time.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include <netdb.h>
#include <map>
#include <poll.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <strings.h>

/* colors */
#define RESET "\033[0m"
#define RED "\033[0;31m"
#define YELLOW "\033[0;33m"
#define GREEN "\033[0;32m"
#define BLACK "\033[0;30m"
#define WHITE "\033[0;37m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"

#define BUFFER_SIZE 20
#define MAX_CLIENTS 100

#define CHANNEL_LIMIT 100

#include "Client.hpp"
#include "Channel.hpp"

class Client;

class Irc
{
private:
    char *_passWord;
    int _port;
    std::string _serverName;
    int _serverSocket, _newSocket;
    struct sockaddr_in _server_addr;
    std::vector<pollfd> _pollfds;
    std::map<int, Client> _clients;
    std::map<int, Client>::iterator it;

public:
    std::map<std::string, Channel> _channels;

    // Irc constructor
    Irc(int port, char *password);

    void getClientIPAddress(int);

    /**
     * @brief socket() Create server socket and initialize it
     *
     * @param INADDR_ANY : the server listening on all available network interfaces
     * @param AF_INET : the addr family for IPv4
     * @param  htons : convert to network byte order (big-endian)
=         * @return zero on success
     */
    void createSocket();

    /**
     * @brief here we allow the server socket fd to be reusable
     *
     * @param AF_INET : IPv4 Internet protocol
     * @param SOCK_STREAM : sock type, ensure reliable, two-way, without loss or duplication connection
     * @param IPPROTO_TCP : tcp rotocol
     * @param SOL_SOCKET : the opteions are the set to the socket level
     * @param SO_REUSEADDR : support the use of the local address
     * if we set the the opt to 0 we will disable this option
     * @return zero on success
     */
    void settingsockopt();

    /**
     * @brief here we set the server socket to be nonbloking
     *
     * @param F_SETFL : file status flag
     * @param O_NONBLOCK : socket operations wait
     * @return zero on success
     */
    void nonBlockFd();

    /**
     * @brief here we set the server socket to be nonbloking
     *
     * @param F_SETFL : file status flag
     * @param O_NONBLOCK : socket operations wait
     * @return zero on success
     */
    void bindSocket();

    // listening for the clients (wait for incoming connection from the client)
    void listeningToClients();

    /**
     * @brief Establish connections and start communication
     *
     * @return poll() returns number of elements in the pollfds whose revents,
     * 0 on timeout -1 on error
     */
    void runServer();

    /**
     * @brief Accept a new client connection
     *
     * @param sockfd : socket that has been listening for connections after a
     * @param addr : socket operations wait
     * @param POLLIN : monitor the file descriptor for incoming data that is ready to be read
     * @return zero on success
     */
    void addClient();

    /**
     * @brief check for clients activities and handle them
     *
     * @param flags : in this case '0' , recv behave like read()
     * @return socket stream sends zero bytes to notifiy the server that the connection has ended
     * -1 when an error occurred
     * count of bytes sent by the socket
     */
    void Handle_activity();

    // print using a color and % exit
    void printc(std::string, std::string, int);

    // buffer the received message in the client message
    void recvClientsMsg(Client &, std::string);

    // handleLogTime
    static void handleLogTime(Client &);

    // handleBot
    static void handleQuotes(Client &);

    // handleBot
    static void handleBot(Client &, std::string);
};

void        *dccFileTransfer(void *arg);

time_t                   time_teller();
std::string              time_to_string(time_t timeVal);
bool                     channel_name_is_valid(std::string &channel_name);
bool                     valid_sp_character(int c);
bool                     is_valid_topic(std::string &new_topic);
std::string              trim(std::string &str, std::string charstotrim);
std::string              extract_channel_name(std::string& command);
bool                     channel_exist(std::map<std::string, Channel>& channels, std::string &needle);
bool                     Create_channel_join(Client &client, std::map<std::string, Channel>& channels, std::string& new_channel_name, std::map<int, Client> &clients);
void                     set_topic(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients);
void                     handle_Join(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients);
std::string              extract_topic(std::string& command);
bool                     channel_join(Client &client, std::map<std::string, Channel>& channels, std::string& new_channel_name, std::map<int, Client> &clients);
void                     send_message(int clientSocket, std::string message);
void                     send_names_list(Client &client, Channel &channel);
void                     leave_channel(std::string command, Client &client, std::map<std::string, Channel>& channels);
void                     quit_server(Client &client, std::map<int, Client> &clients, std::map<std::string, Channel>& channels);
void                     kick_user(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients);
void                     invite_user(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients);
void                     mode(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients);
std::string              extract_target(std::string& command);
std::string              extract_reason(std::string& command);
bool                     is_valid_password(std::string pass);
bool                     valid_option(std::string &option);
std::vector<std::string> parser(std::string &full_command, char dilimeter);
long int                 contains_only_nums(std::string string);
void                     mode(std::string command, Client &client, std::map<std::string, Channel>& channels, std::map<int, Client> &clients);
void                     mode_one_param(Client &excuter);
void                     mode_two_params(std::map<std::string, Channel>& channels, Client &excuter, std::string &channel_name);
void                     mode_with_params(std::map<std::string, Channel>& channels, Client &excuter, std::string &channel_name, std::string &mode, std::map<int, Client> &clients, std::string &last_param);
void                     send_mode_info(Client &excuter, Channel &channel);
int                      get_client_fd(std::string &client_name, std::map<int, Client> &clients);
void                     monitoring(std::map<std::string, Channel> &channels, std::map<int, Client> &clients);
void                     oper(std::string command, Client &client);
bool                     valid_full_option(std::string &option);
void                     execute_mode(std::map<std::string, Channel>& channels, Client &excuter, std::string &channel_name, std::string &mode, std::map<int, Client> &clients, std::string &last_param);
std::vector<std::string> mode_parser(std::string &full_command, char dilimeter);
void                     trigbot(std::string command, Client &client, std::__1::map<int, Client> &clients);



void leak();