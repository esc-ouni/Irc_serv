
EXEC    = ircserv
CC      = c++
RM      = rm -rf
#FLAGS   = -fsanitize=address,undefined
H_FILES = ${shell ls headers/*.hpp}
C_FILES = ${shell ls src/*/*.cpp} ${shell ls src/*.cpp}
OBJS    = $(C_FILES:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJS) $(H_FILES)
	@$(CC) $(FLAGS) $(OBJS) -o $(EXEC)
<<<<<<< HEAD
	@clear
	# ./ircserv 6667 pass
=======
	#@clear
	./ircserv 6667 pass

>>>>>>> fmaster

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
	@clear && echo "==All_created_files_deleted=="

re: fclean all
	@clear && echo "==Compilation_reseted========"

Transfer:
	@c++ fileTransfer/filetransfer.cpp -o fileTransfer/transfer
	@clear
	@echo "./fileTransfer/transfer <Pathfile>"

Boot:
	@c++ bot/bot.cpp -o bot/bot
	@clear
	@echo "./bot/bot <IP> <PORT>"

.PHONY: all clean fclean re transfer
