/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 17:33:13 by idouni            #+#    #+#             */
/*   Updated: 2023/12/17 17:33:28 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/Irc.hpp"
#include "../../headers/Channel.hpp"
#include "../../headers/commands.hpp"

void oper(std::string command, Client &client){
    std::vector<std::string> args = parser(command, ' ');
    std::string name, password;

    int argc = args.size();

    if (argc == 3){
        name     = args.at(1);
        password = args.at(2);
        if (name != "admin"){
            send_message(client.get_fd(), ERR_NOOPERHOST(client.get_nickname()));
            return ;
        }
        if (password != "admin"){
            send_message(client.get_fd(), ERR_PASSWDMISMATCH(client.get_nickname()));
            return ;
        }
        send_message(client.get_fd(), RPL_YOUREOPER(client.get_nickname()));
        client.promote_IRC_op();
    }
    else
        send_message(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nickname(), "OPER"));
    return ;
};