
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
	@clear
	./ircserv 6667 pass

%.o: %.cpp $(H_FILES)
	$(CC) $(FLAGS) -c $< -o $@ 

clean:
	$(RM) $(OBJS)
	@clear && echo "==Object_files_deleted======="

fclean: clean
	$(RM) $(EXEC)
	$(RM) .vscode
	$(RM) *.dSYM
	@clear && echo "==All_created_files_deleted=="

re: fclean all
	@clear && echo "==Compilation_reseted========"

bonus:
	@c++ fileTransfer/filetransfer.cpp -o fileTransfer/bot
	@clear
	@echo ./fileTransfer/bot

.PHONY: all clean fclean re
