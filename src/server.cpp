#include "../headers/Irc.hpp"
#include "../headers/Channel.hpp"
#include "../headers/commands.hpp"


Irc::Irc(int port, char *password)
{
    _passWord = password;
    _serverName = ":MSN ";
    _port = port;

    if (_port < 1024 || _port > 65535)
        printc("The port is out of rang", RED, 1);

    createSocket();
    settingsockopt();
    nonBlockFd();
    bindSocket();
    listeningToClients();

    std::cout << BLACK << "IRC Server is running on port : " << _port << RESET << std::endl;
}

void Irc::createSocket()
{
    _serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_serverSocket < 0)
        printc("ERROR : socket", RED, 1);

    _server_addr.sin_addr.s_addr = INADDR_ANY;
    _server_addr.sin_family = AF_INET;
    _server_addr.sin_port = htons(_port);
}

void Irc::settingsockopt()
{
    int opt = 1;
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        printc("ERROR : setsockopt", RED, 1);
    if (setsockopt(_serverSocket, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt)) == -1)
        printc("ERROR : setsockopt", RED, 1);

}

void Irc::nonBlockFd()
{
    if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) == -1)
        printc("ERROR : fcntl", RED, 1);
}

void Irc::bindSocket()
{

    if (bind(_serverSocket, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) < 0)
        printc("ERROR : bind", RED, 1);
}

void Irc::listeningToClients()
{
    if (listen(_serverSocket, MAX_QUUED_CLIENTS) < 0)
        printc("ERROR : listen", RED, 1);
}

void Irc::runServer()
{
    pollfd serverPoll;
    serverPoll.fd = _serverSocket;
    serverPoll.events = POLLIN;
    _pollfds.push_back(serverPoll);

    while (server_turn_off == false)
    {
        if (poll(&_pollfds[0], _pollfds.size(), -1) == -1)
        {
            closePollfd(_pollfds);
        }

        if (_pollfds[0].revents & POLLIN)
            addClient();

        Handle_activity();
    }
}

void Irc::addClient()
{
    struct sockaddr_in client_addr;
    socklen_t       addr_size = sizeof(client_addr);

    _newSocket = accept(_serverSocket, (sockaddr *)&client_addr, &addr_size);
    if (_newSocket < 0)
        printc("Error accepting client connection", RED, 1);

    Client new_client(_newSocket);

    new_client.set_pass(_passWord);
    new_client.setHasPassword(false);
    new_client.setHasNickname(false);
    new_client.set_client_host(inet_ntoa(client_addr.sin_addr));
    pollfd client_pollfd = {_newSocket, POLLIN, 0};
    _pollfds.push_back(client_pollfd);
    _clients.insert(std::pair<int, Client>(_newSocket, new_client));

    std::cout << GREEN << "[Server] Added client #" << _newSocket
              << " successfully" << RESET << std::endl;
}

void Irc::printc(std::string msg, std::string color, int ex)
{
    std::cerr << color << msg << RESET << std::endl;
    if (ex)
        exit(EXIT_SUCCESS);
}

void Irc::Handle_activity()
{
    for (size_t i = 1; i < _pollfds.size(); i++)
    {
        if (_pollfds[i].revents & POLLIN)
        {
            char buffer[BUFFER_SIZE];
            bzero(buffer, sizeof(buffer));
            int count = recv(_pollfds[i].fd, buffer, BUFFER_SIZE - 1, 0);
            if (count == 0)
            {
                std::cout << YELLOW << "Client " << _pollfds[i].fd << " disconnected.\n";
                std::cout << PURPLE << "Total clients : " << _pollfds.size() - 2 << RESET << std::endl;
            }
            else if (count < 0)
            {
                std::cerr << RED << "Error reading from client " << _pollfds[i].fd << "\n";
                std::cout << PURPLE << "Total clients : " << _pollfds.size() - 2 << RESET << std::endl;
            }
            else
            {
                std::string message(buffer);
                it = _clients.find(_pollfds[i].fd);

                if (it != _clients.end())
                    recvClientsMsg(it->second, message);
                if (it->second.get_buffer().find('\n') != std::string::npos)
                {
                    if (it->second.get_buffer().length() >= 512)
                    {
                        std::string msg = ": 421 ! more than 512 characters\r\n";
                        send(it->second.get_fd(), msg.c_str(), strlen(msg.c_str()), 0);
                        it->second.set_buffer("");
                        continue;
                    }
                    excute_command(it->second.get_buffer(), it->second, _channels, _clients);
                    std::cout << BLUE << "Client [" << it->second.get_fd() << "] : "
                              << it->second.get_buffer() << RESET << std::flush;
                    it->second.set_buffer("");
                }
            }
            if (_pollfds[i].revents & POLLIN && count <= 0)
            {
                close(_pollfds[i].fd);
                _clients.erase(_pollfds[i].fd);
                _pollfds.erase(_pollfds.begin() + i);
            }
        }
    }
}

void Irc::recvClientsMsg(Client &client, std::string buffer)
{
    client.add_buffer(buffer);
}

void closePollfd(std::vector<pollfd>& pollfd) {

    for (size_t i = 0; i < pollfd.size(); i++)
    {
        close(pollfd[i].fd);
    }
    exit(EXIT_SUCCESS);
}

int IsDigit(char *argv)
{
    for (int i = 0; argv[i] != '\0'; i++)
    {
        if (!std::isdigit(argv[i]))
            return (1);
    }
    return (0);
}

// std::string downloadPath(std::string str)
// {
//     std::string result;
//     for (size_t i = 0; i < str.length(); i++)
//     {
//         if (str[i] != '\n' && str[i] != '\r' \
//            && str[i] != '\t' && str[i] != '\v'\
//            && str[i] != '\f' && str[i] != ' ' \
//            && str[i] != '\'' && str[i] != '"'\
//            && str[i] != ':')
//         {
//             result += str[i];
//         }
//     }
//     return (result);
// }

// void Irc::handleBot(Client &client, std::string command)
// {
//     std::string path = downloadPath(command.substr(9, command.length() - 9));
//     std::string msg;
//     char buffer[1000];
//     std::string file;
//     FILE *fd = fopen(path.c_str(), "rb");
//     if (fd == NULL)
//     {
//         msg = "PRIVMSG " + client.get_nickname() + " :" + "Error \
//         : opening file / file do not exist" + "\r\n";
//         send(client.get_fd(), msg.c_str(), msg.size(), 0);
//         return;
//     }
//     while (!feof(fd))
//     {
//         int size = fread(&buffer, 1, 1000, fd);
//         if (size < 0)
//             break;
//         file.append(buffer, size);
//     }
//     fclose(fd);

//     msg = "PRIVMSG " + client.get_nickname() + " :" + '\x01' + \
//     "DCC SEND " + path.c_str() + " 0 9999 " + std::to_string(file.size()) + '\x01';
//     msg += "\r\n";
//     send(client.get_fd(), msg.c_str(), msg.size(), 0);
// }
