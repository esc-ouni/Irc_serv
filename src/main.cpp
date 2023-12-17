#include "../headers/Irc.hpp"
#include <cstdlib>

std::string filteredString(std::string str);

bool server_turn_off = false;

// void f_leak(){
//     std::string msg = "lsof -p ";
//     msg += std::to_string(getpid());
//     system(msg.c_str());
// }
//     atexit(f_leak);


void signal_handler(int signal)
{
	(void)signal;
	server_turn_off = true;
    std::cout << RED << "server turned off" << RESET << std::endl;
}

void closePollfd(std::vector<pollfd>& pollfd) {

    for (size_t i = 0; i < pollfd.size(); i++)
    {
        close(pollfd[i].fd);
    }
    exit(EXIT_SUCCESS);
}

static int IsDigit(char *argv)
{
    for (int i = 0; argv[i] != '\0'; i++)
    {
        if (!std::isdigit(argv[i]))
            return (1);
    }
    return (0);
}

int main(int ac, char *argv[])
{
    if (ac != 3 || IsDigit(argv[1]) || argv[2] == NULL || !strlen(argv[2]))
    {
        std::cerr << RED << "try ./ircserv [port] [password]" << RESET << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string pass = filteredString(argv[2]);
    if (filteredString(pass).empty())
    {
        std::cerr << RED << "try other password" << RESET << std::endl;
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, signal_handler);
    std::vector<pollfd> getPollfds;

    try
    {
        Irc irc(std::atoi(argv[1]), argv[2]);
        irc.runServer();
        getPollfds = irc.getPollfds();
        closePollfd(getPollfds);
    }
    catch (const std::exception &e)
    {
        closePollfd(getPollfds);
        std::cerr << e.what() << '\n';
    }

    return (EXIT_SUCCESS);
}
