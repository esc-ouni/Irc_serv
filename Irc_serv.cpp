/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc_serv.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:22:37 by idouni            #+#    #+#             */
/*   Updated: 2023/11/22 14:24:58 by idouni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc_serv.hpp"

void disconnect(int opened_socket){
	close(opened_socket);
};

int connect_client(int port){
	int sock_id = socket(PF_INET, SOCK_STREAM, getprotobyname("tcp")->p_proto);
	if (sock_id == -1)
		throw (std::logic_error("Error Creating A Socket\n"));

	struct sockaddr_in *sockaddr_in = new struct sockaddr_in;

	sockaddr_in->sin_family = AF_INET;
	sockaddr_in->sin_port = htons(port);
	sockaddr_in->sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sock_id, (const struct sockaddr *)sockaddr_in , sizeof(sockaddr)) == -1){
		disconnect(sock_id);
		throw (std::logic_error("Error Binding A Socket\n"));
	}
	
	if (listen(sock_id, 1) == -1){
		disconnect(sock_id);
		throw (std::logic_error("Error Listening On Socket\n"));	
	}

	std::cout << "READY TO CONNECT ..." << std::endl;
	int client = accept(sock_id, NULL, NULL);
	if ( client == -1){
		disconnect(sock_id);
		throw (std::logic_error("Error Accepting Request\n"));
	}
	disconnect(sock_id);
	delete sockaddr_in;
	return (client);	
}


int run_server(int argc, char const *argv[]){
	std::cout << "LOADING ..." << std::endl;

	if (argc != 2)
		throw (std::logic_error("Bad Aruments, try : ./ircserv <port> \n"));

	int port = std::atoi(argv[1]);
	if (port < 1024 || port >65535)
		throw (std::logic_error("Not A Valid Port Number\n"));
	
	return (connect_client(port));

};

void handel_client(int client, int port){
	std::cout << "Client 1, CONNECTED SUCCESSFULLY" << std::endl << std::endl;
	
	// char buff[] = "CAP * LS :\0";
	// send(client, buff ,sizeof(buff) , 0);
	// while(true)
	// 	continue;
	
	char recieved[1024];
	bzero(recieved, sizeof(recieved));
	int readed = recv(client, recieved, 1023, 0);
	int msg_length = strlen(recieved);
	
	while (true){
		if (msg_length > 0){
			recieved[strlen(recieved) - 1] = '\0';
			std::cout << "Client 1 sent : [" << recieved <<"] " << std::endl;
			bzero(recieved, sizeof(recieved));
			readed = 0;
			msg_length = 0;
		}
		else if (readed == -1){
			disconnect(client);
			std::cout << std::endl;
			throw (std::logic_error("Error During Waiting For Requests\n"));		
		}
		else if (!readed){
			disconnect(client);
			std::cout << std::endl;
			std::cout << "CLIENT DISCONNECTED, ..."<< std::endl;
			client = connect_client(port += 1);
		}
		bzero(recieved, sizeof(recieved));
		readed = recv(client, recieved, 1023, 0);
		recieved[strlen(recieved)] = '\0';
		msg_length = strlen(recieved);
	}
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
		handel_client(client, std::atoi(argv[1]));
	}
	catch(const std::exception& e){
		std::cerr << e.what() << '\n';
		exit(-1);
	}
	
	
	return (0);
}
