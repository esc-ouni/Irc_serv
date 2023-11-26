#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

std::string filteredString(std::string str)
{
    std::string filteredstr;
    for (int i = 0; i < (int)str.length(); i++)
    {
        if (str[i] != '\n' && str[i] != '\r' \
           && str[i] != '\t' && str[i] != '\v'\
           && str[i] != '\f' && str[i] != ' ')
        {
            filteredstr += str[i];
        }
    }
    return filteredstr;
}

static bool checkForma(const std::string &username, std::string &user)
{
    std::istringstream iss(username);
    std::string word;
    int position = 1;
    while (iss >> word)
    {
        if (position == 1 || position == 4)
        {
            user = position == 1 ? word : user;
        }
        else if (position == 2)
        {
            if (word != "0")
            {
                return false;
            }
        }
        else if (position == 3)
        {
            if (word != "*")
            {
                return false;
            }
        }
        else
        {
            return false;
        }

        position++;
    }
    return position == 5;
}

void user(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> clients)
{
    (void)channels;
    (void)clients;
    std::stringstream ss;
    ss << client.get_fd();
    std::string str = ss.str();
    std::string username = command.substr(5, command.length() - 5);
    std::string user;
    if (client.is_authenticated() == false && client.get_pass() != "" && client.get_nickname() != "")
    {
        if (username.empty())
            send(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "USER").c_str(), ERR_NEEDMOREPARAMS(client.get_nickname(), "USER").length(), 0);
        else if (checkForma(username, user) == false)
            send(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "USER").c_str(), ERR_NEEDMOREPARAMS(client.get_nickname(), "USER").length(), 0);
        else
        {
            client.set_username(user);
            client.set_authenticated(true);
            client.add_buffer_to_send(RPL_WELCOME(str, client.get_nickname()));
        }
    }
}

bool hasInvalidCharacters(const std::string &name)
{
    char firstChar = name[0];
    if (firstChar == '$' || firstChar == ':' || firstChar == '#')
    {
        return true;
    }
    const char invalidChars[] = " ,*?!@.";
    for (size_t i = 0; i < name.size(); ++i)
    {
        for (size_t j = 0; j < sizeof(invalidChars) - 1; ++j)
        {
            if (name[i] == invalidChars[j])
            {
                return true;
            }
        }
    }
    return false;
}

void nick(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> clients)
{
    (void)channels;
    std::string nickname = filteredString(command.substr(5, command.length() - 5));
    if (client.is_authenticated() == false && client.get_pass() != "" && client.get_nickname() == "")
    {

        if (nickname.empty())
        {
            send(client.get_fd(), ERR_NONICKNAMEGIVEN , sizeof(ERR_NONICKNAMEGIVEN), 0);
        }

        else if (client_already_exist(nickname, clients))
        {
            send(client.get_fd(), ERR_NICKNAMEINUSE(client.get_nickname(), nickname).c_str(), ERR_NICKNAMEINUSE(client.get_nickname(), nickname).length(), 0);
        }
        else if (hasInvalidCharacters(nickname))
        {
            send(client.get_fd(), ERR_ERRONEUSNICKNAME(client.get_nickname(), nickname).c_str(), ERR_ERRONEUSNICKNAME(client.get_nickname(), nickname).length(), 0);
        }
        else
        {
            client.set_nickname(nickname);
        }
    }
    else
    {
        if (client.is_authenticated() == true && !nickname.empty() && !client_already_exist(nickname, clients) && !hasInvalidCharacters(nickname))
        {
            client.set_old_nick(client.get_nickname());
            client.set_nickname(nickname);
            client.set_buff_to_send(RPL_NICK(client.get_old_nick(), client.get_username(), nickname));
        }
        else if (client.is_authenticated() == true && nickname.empty())
            send(client.get_fd(), ERR_NONICKNAMEGIVEN , sizeof(ERR_NONICKNAMEGIVEN), 0);
    }
}

void pass(std::string password, std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> &clients)
{
    (void)channels;
    (void)clients;
    std::string pass = filteredString(command.substr(5, command.length() - 5));
    if (client.is_authenticated() == false && client.get_pass() == "")
    {
        if (pass.empty())
        {
            send(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "PASS").c_str(), ERR_NEEDMOREPARAMS(client.get_nickname(), "PASS").length(), 0);
        }
        else if (pass != password)
            send(client.get_fd(), ERR_PASSWDMISMATCH(client.get_nickname()).c_str(), ERR_PASSWDMISMATCH(client.get_nickname()).length(), 0);
        else
            client.set_pass(pass);
    }
}

void excute_command(std::string password , std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> &clients)
{
    if (command.substr(0, 4) == "PASS")
        pass(password, command, client, channels, clients);
    else if (command.substr(0, 4) == "NICK")
        nick(command, client, channels, clients);
    else if (command.substr(0, 4) == "USER")
        user(command, client, channels, clients);


//     else if (command.substr(0, 4) == "JOIN" && client.is_authenticated())
//     {
//         join(command, client, channels);
//     }
//     else if (command.substr(0, 4) == "KICK" && client.is_authenticated())
//     {
//         kick(command, client, channels);
//     }
//     // INVITE
//     else if (command.substr(0, 6) == "INVITE" && client.is_authenticated())
//     {
//         invite(command, client, channels, clients);
//     }
//     else if (command.substr(0, 4) == "MODE" && client.is_authenticated())
//     {
//         mode(command, client, channels);
//     }
    // else if (command.substr(0, 7) == "PRIVMSG" && client.is_authenticated())
    // {
    //     privmsg(command, client, clients, channels);
    //     std::cout << "PRIVMSG" << std::endl;
    // }
//     else if (filteredString(command) == "LOGTIME" && client.is_authenticated())
//     {
//         Irc::handleLogTime(client);
//     }
//     else if (filteredString(command) == "QUOTES" && client.is_authenticated())
//     {
//         Irc::handleQuotes(client);
//     }
//     else if (filteredString(command) == "DOWNLOAD" && client.is_authenticated())
//     {
//         Irc::handleBot(client);
//     }
//     else if (command.substr(0, 5) == "TOPIC" && client.is_authenticated())
//     {
//         topic(command, client, channels, clients);
//     }

//     else if (filteredString(command.substr(0, 4))!= "QUIT")
//     {
        
//         send(client.get_fd(), ERR_UNKNOWNCOMMAND(command.substr(0, command.find(" "))).c_str(), ERR_UNKNOWNCOMMAND(command.substr(0, command.find(" "))).length(), 0);
//     }


//     if (client.is_authenticated() && !client.get_buff_to_send().empty())
//     {
//         send(client.get_fd(), client.get_buff_to_send().c_str(), client.get_buff_to_send().length(), 0);
//         client.set_buff_to_send("");
//     }
}