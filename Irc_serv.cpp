/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc_serv.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:22:37 by idouni            #+#    #+#             */
/*   Updated: 2023/11/22 11:50:51 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc_serv.hpp"

int run_server(int argc, char const *argv[]){
	std::cout << "Loading ..." << std::endl;

	if (argc != 2)
		throw (std::logic_error("Bad Aruments, try : ./ircserv <port> \n"));

	int port = std::atoi(argv[1]);
	if (port < 1024 || port >65535)
		throw (std::logic_error("Not A Valid Port Number\n"));
	
	int sock_id = socket(PF_INET, SOCK_STREAM, getprotobyname("tcp")->p_proto);
	if (sock_id == -1)
		throw (std::logic_error("Error Creating A Socket\n"));

	struct sockaddr_in *sockaddr_in = new struct sockaddr_in;


	
	sockaddr_in->sin_family = AF_INET;
	sockaddr_in->sin_port = htons(port);
	sockaddr_in->sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sock_id, (const struct sockaddr *)sockaddr_in , sizeof(sockaddr)) == -1)
		throw (std::logic_error("Error Binding A Socket\n"));
	

	if (listen(sock_id, 1) == -1)
		throw (std::logic_error("Error Listening On Socket\n"));

	std::cout << "READY TO CONNECT ..." << std::endl;
	int client = accept(sock_id, NULL, NULL);
	if ( client == -1)
		throw (std::logic_error("Error Accepting Request\n"));
	
	
	return (client);
};


void handel_client(int client){
	std::cout << "Client 1, CONNECTED SUCCESSFULLY" << std::endl << std::endl;
	char recieved[1024];
	bzero(recieved, sizeof(recieved));
	int readed = read(client, recieved, 1023);
	while (true){
		if (readed > 0){
			recieved[strlen(recieved) - 2] = '\0';
			std::cout << "Client 1 sent : [" << recieved <<"] " << std::endl;
			bzero(recieved, sizeof(recieved));
			readed = 0;
		}
		else if (readed == -1)
			throw (std::logic_error("Error Reading Request\n"));		
		bzero(recieved, sizeof(recieved));
		readed = read(client, recieved, 1023);
	}
	close(client);
}


int main(int argc, char const *argv[]){
	int client;
	
	try{
		client = run_server(argc, argv);
	}
	catch(const std::exception& e){
		std::cerr << e.what() << '\n';
		exit(-1);
	}
	try{
		handel_client(client);
	}
	catch(const std::exception& e){
		std::cerr << e.what() << '\n';
		exit(-1);
	}
	
	
	return (0);
}
