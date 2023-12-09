/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 19:34:15 by idouni            #+#    #+#             */
/*   Updated: 2023/12/09 12:59:22 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Irc.hpp"
#include "Channel.hpp"
// #define user_forma(nickname, username, adress) (":" + nickname + "!" + username + "@" + adress)
#define user_forma(nickname, username, adress) (":" + nickname + "!" + username + "@localhost")

// #define user_forma(nickname, username) (":" + nickname + "!" + username + "@localhost")
#define ERR(command) (": 421 !" + command + "\r\n")//
#define ERR_NONICKNAMEGIVEN ": 431 :There is no nickname.\r\n"
#define ERR_ERRONEUSNICKNAME(nickname) (": 432 ! " + nickname + " :Erroneous nickname\r\n")
// #define RPL_NICK(oclient, uclient, client) (":localhost " + oclient + "!" + uclient + "@localhost NICK " + client + "\r\n")
#define RPL_NICK(oclient, uclient, client) (":" + oclient + "!" + uclient + "@localhost NICK " + client + "\r\n")
#define ERR_NICKNAMEINUSE(client, nickname) (": 433 !" + client + " " + nickname + " :Nickname is already in use.\r\n")//
#define RPL(msg, nickname) (": 001 " + nickname + " : " + msg + "\r\n")
#define RPL_WELCOME(user_forma, nickname) (": 001 " + nickname + " :Welcome " + nickname + " to the Internet Relay Chat " + user_forma + "\r\n")
#define RPL_YOURHOST(client, localhost) (": 002 " + client + " :Your host is " + localhost + "\r\n")
#define RPL_CREATED(client, datetime) (": 003 " + client + " :This server was created " + datetime + "\r\n")
# define RPL_MYINFO(client, servername, version, user_modes, chan_modes, chan_param_modes) (":localhost 004 " + client + " " + servername + " " + version + " " + user_modes + " " + chan_modes + " " + chan_param_modes + "\r\n")
# define RPL_ISUPPORT(client, tokens) (":localhost 005 " + client + " " + tokens " :are supported by this server\r\n")


// channels
// #define RPL_JOIN(user_forma, client, channel) (user_forma + " JOIN :" + channel + "\r\n")
// #define RPL_KICK(client, channel, kicked, reason) (":@" + client + " KICK " + channel + " " + kicked + " :" + reason + "\r\n")
// #define ERR_BADCHANNELKEY(client, channel) (":localhost 475 " + client + " " + channel + " :Cannot join channel (+k)\r\n")
// #define ERR_CHANNELISFULL(client, channel) (":localhost 471 " + client + " " + channel + " :Cannot join channel (+l)\r\n")
#define ERR_IN  VITEONLYCHAN(client, channel) (":localhost 473 " + client + " " + channel + " :Cannot join channel (+i)\r\n")
// INVITE
#define RPL_INVITING(client, nickname, channel) (":localhost 341 " + client + " " + nickname + " " + channel + "\r\n")
// 301
#define RPL_AWAY(client, nickname, away_message) (":localhost 301 " + client + " " + nickname + " :" + away_message + "\r\n")

// end channels
#define ERR_PASSWDMISMATCH(client) (":localhost 464 " + client + " :Password incorrect.\r\n")//
// #define ERR_PASSWDMISMATCH(client) (":localhost 464 " client " :Password incorrect.\r\n")

#define RPL_MYINFO(client, localhost, version, user_modes, chan_modes, chan_param_modes) (":localhost 004 " + client + " " + localhost + " " + version + " " + user_modes + " " + chan_modes + " " + chan_param_modes + "\r\n")

// privmsg
#define ERR_NORECIPIENT(client) ("411 " + client + " :No recipient given PRIVMSG\r\n")
#define ERR_NOTEXTTOSEND(client) ("412 " + client + " :No text to send\r\n")
#define ERR_CANNOTSENDTOCHAN(client, channel) ("404 " + client + " " + channel + " :Cannot send to channel\r\n")
#define RPL_PRIVMSG(nick, username, target, message) (":" + nick + "!" + username + "@localhost PRIVMSG " + target + "  :" + message + "\r\n")

// #define RPL_PRIVMSG(nick, username, target, message) (":" + nick + "!" + username + "@localhost PRIVMSG " + target + " " + message + "\r\n")



// :OldNick!UserHost NICK NewNick
#define RPL_NICKCHANGED(Oldnick, Newnick) (":" + Oldnick + "!" + "client_host"+ " NICK " + Newnick + "\r\n");

///IMPT

// to the excuter :ServerName 341 InviterNick InvitedNick #Channel
#define  RPL_INVITESEND(client, nick, channel) (":localhost 341 " + client + " " + nick + " " + channel + "\r\n")
        
// to the invitee :InviterNick!InviterUser@InviterHost INVITE InvitedNick :#Channel
#define  RPL_INVITENOTIFY(client, nick, channel) (":" + client + "!InviterUser@InviterHost" + " INVITE " + nick+ " :" + channel + "\r\n")


// :ServerName 443 YourNick User #Channel :is already on channel
#define  ERR_USERNOTINCHANNEL(client, nick, channel) (":localhost 443 " + client +" " + nick + " " + channel + " :user not on channel.\r\n")
#define  ERR_NEEDMOREPARAMS(client, command) (":localhost 461 " + client + " " + command + " :Not enough parameters.\r\n")
#define  ERR_NOTONCHANNEL(client, channel) (":localhost 442 " + client + " " + channel + " :You're not on that channel.\r\n")

#define  RPL_CHANNELMODEIS(client, channel, mode) (":localhost 324 " + client + " " + channel + " " + mode + "\r\n")
#define  RPL_CREATIONTIME(client, channel, datetime) (":localhost 329 " + client + " " + channel + " " + datetime + "\r\n") 
#define  MODE_CHANGED(client, channel, mode, last_param) (":" + client + "!UserHost MODE " + channel + " " + mode + " " + last_param + "\r\n")
#define  ERR_CHANOPRIVSNEEDED(client, channel) (":localhost 482 " + client + " " + channel + " :You're not channel operator.\r\n")

#define  ERR_UMODEUNKNOWNFLAG(client, channel) (":localhost 501 " + client + " :Unknown MODE flag\r\n")
#define  ERR_INVALIDMODEPARAM(client, channel, mode) (":localhost 696 " + client + " " + channel + " " + mode + " :Invalid MODE parameter\r\n")
#define  ERR_UNKNOWNMODE(client, channel, mode) (":localhost 472 " + client + " " + mode + " :is unknown mode char to me\r\n")
#define  ERR_INVALIDKEY(client, channel) (":localhost 525 " + client + " " + channel + " :Cannot join channel (+k) - bad key\r\n")
#define  ERR_CHANNELISFULL(client, channel) (":localhost 471 " + client + " " + channel + " :Cannot join channel (+l) - channel is full\r\n")
#define  ERR_INVITEONLYCHAN(client, channel) (":localhost 473 " + client + " " + channel + " :Cannot join channel (+i) - invite only\r\n") 

// :ServerName 401 OperatorNick InvitedNick :No such nick/channel
#define  ERR_NOSUCHNICK(client, target) (":localhost 401 " + client + " " + target + " :No such nick/channel\r\n")
#define  ERR_NOSUCHCHANNEL(client, channel) (":localhost 403 " + client + " " + channel + " :No such channel\r\n")



#define  ERR_USERONCHANNEL(client, nick, channel) (":localhost 443 " + client + " " + nick + " " + channel + " is already on channel\r\n")
// KICK
// :KickerNick!KickerUser@KickerHost KICK <channel> <user> :[reason]
#define RPL_KICK(client, channel, kicked, reason) (":" + client + "!" + "Kicker@KickerHost KICK " + channel + " " + kicked + " :" + reason + "\r\n");

// QUIT
// :Nickname!Username@Host QUIT :Quit Message
#define RPL_NOTIFYQUIT(client, message) (":" + client + "!" + "client_host"+ " QUIT :" + message + "\r\n")

// PART
// :ClientNick!ClientHost PART #channel
#define RPL_NOTIFYPART(client, channel) (":" + client + "!" + "client_host" + " PART " + channel + "\r\n")


// :Nickname!Username@Host JOIN #channel
#define RPL_NOTIFYJOIN(client, channel) (":" + client + "!" + client + "@" + "client_host"+ " JOIN " + channel + "\r\n")

//JOIN
#define RPL_JOIN(client, channel) (":" + client + "!~" + client + "@" + "client_host"+ " JOIN :" + channel + "\r\n")

// :localhost 353 YourNickname = #ChannelName :User1 User2 User3...
#define RPL_NAMREPLY(client, channel, all_users) (": 353 " + client + " = " + channel + " :" + all_users + "\r\n")

// :localhost 366 YourNickname #ChannelName :End of /NAMES list
#define RPL_ENDOFNAMES(client, channel) (": 366 " + client + " " + channel + " :End of /NAMES list\r\n")

// msg channel
#define RPL_CHANNMSG(client, host, channel, message) (":" + client + "!" + host + " PRIVMSG " + channel + "  :" + message + "\r\n")
// TOPIC
// :server 332 user #channel :topic
#define RPL_TOPIC(client, channel, topic) (": 332 " + client + " " + channel + " :" + topic + " \r\n")

// :server 333 user #channel setter_nick timestamp
// ": 333 " + client.get_nickname() + " " + channel_name + " " + channels[channel_name].get_topic_setter() + " " +  time_to_string(time_teller()) + "\r\n";

#define RPL_TOPICWHOTIME(client, channel, setter, datetime) (": 333 " + client + " " + channel + " "  + setter + " " + datetime + " \r\n")

// :localhost 331 YourNickname #ChannelName :No topic is set
#define RPL_NOTOPIC(client, channel, datetime) (": 333 " + client + " " + channel  + "setter_here" + datetime + " \r\n")

// Broadcast Topic Change
#define RPL_TOPICCHANGE(client, channel, topic) (":" + client + "!"+ "UserHost" + " TOPIC " + channel_name + " :" +  topic + " \r\n")













/**
 * @brief execute the command given by the client
 * @param command the command to execute
 * @param client the client who sent the command
 * @param channels the channels of the server
 * @return void
 */
void excute_command(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> &clients);
void privmsg(std::string message, Client &client, std::map<int, Client> &clients, std::map<std::string, Channel> &channels);
// void broadcastTochannel(Client client, std::string message, std::string channel, std::map<std::string, Channel> &channels);
// void set_topic(std::string &command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> &clients);
/**
 * @brief join a channel
 * @param client the client who wants to join the channel
 * @param channels the channels of the server
 * @param channel_name the name of the channel to join
 * @param command the command received from the client
 * @return int 0 if success, 1 if error
 */

int join(std::string command, Client &client, std::map<std::string, Channel> &channels);
/**
 * Checks if a client with the given nickname already exists in the map of clients.
 * @param nickname The nickname to check for.
 * @param clients The map of clients to search in.
 * @return True if a client with the given nickname already exists in the map of clients, false otherwise.
 */
bool client_already_exist(std::string nickname, std::map<int, Client> clients);

Client &get_client_by_nickname(std::string &nick, std::map<int, Client> &clients);
/**
 * @brief Filters a string by removing any unwanted characters.
 *
 * @param str The string to be filtered.
 * @return The filtered string.
 */
std::string filteredString(std::string str);
// void nick(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> clients);
void user(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> clients);
bool pass(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> &clients);
bool is_multipe_words(std::string message);

/**
 * Kicks a client from a channel.
 *
 * @param command The kick command to execute.
 * @param client The client who executed the command.
 * @param channels The map of channels in the server.
 * @param clients The map of clients in the server.
 */
void kick(std::string command, Client &client, std::map<std::string, Channel> &channels);

/**
 * @brief Invites a client to a channel.
 *
 * @param command The command string.
 * @param client The client who executed the command.
 * @param channels The map of channels.
 * @param clients The map of clients.
 * @return int Returns 0 if the invite was successful, -1 otherwise.
 */
// int invite(std::string command, Client &client, std::map<std::string, Channel> &channels, std::map<int, Client> &clients);

// void mode(std::string command, Client &client, std::map<std::string, Channel> &channels);

std::string filteredString(std::string str);
bool pass(std::string command, Client &client);
void nick(std::string command, Client &client, std::map<int, Client> clients, std::map<std::string, Channel> &channels);
void user(std::string command, Client &client);

