# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: idouni <idouni@student.1337.ma>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/18 09:54:08 by idouni            #+#    #+#              #
#    Updated: 2023/12/21 16:46:45 by idouni           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

EXEC      = ircserv
CC        = c++
RM        = rm -rf
FLAGS     = -std=c++98 -Wall -Wextra -Werror
H_FILES   = ${shell ls headers/*.hpp}
CPP_FILES = ${shell ls src/*/*.cpp} ${shell ls src/*.cpp}
OBJS      = $(CPP_FILES:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJS) $(H_FILES)
	@$(CC) $(FLAGS) $(OBJS) -o $(EXEC)
	@clear && echo "==$(EXEC)_compiled==========="

%.o: %.cpp $(H_FILES)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	$(RM) ./fileTransfer/*.o
	@clear && echo "==Object_files_deleted======="

fclean: clean
	$(RM) $(EXEC)
	$(RM) ./fileTransfer/transfer
	$(RM) .vscode
	$(RM) *.dSYM
	$(RM) Emet_v1
	@clear && echo "==All_created_files_deleted=="

re: fclean all
	@clear && echo "==Compilation_reseted========"

Transfer:
	@c++ fileTransfer/filetransfer.cpp -o fileTransfer/transfer
	@clear
	@echo "./fileTransfer/transfer <Pathfile>"

Bot:
	@c++ $(FLAGS) bot/*.cpp -o ./Emet_v1
	@clear
	@echo "./Emet_v1 <IP> <PORT>"

.PHONY: all clean fclean re transfer Bot
