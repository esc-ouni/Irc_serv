
#include "../headers/Irc.hpp"
#include "../headers/commands.hpp"

bool server_turn_off = false;

void signal_handler(int signal)
{
	(void)signal;
	server_turn_off = true;
    std::cout << RED << "server turned off" << RESET << std::endl;
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
