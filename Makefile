
EXEC    = ircserv
CC      = c++
RM      = rm -rf
# FLAGS   = -std=c++98 -Wall -Wextra -Werror #-fsanitize=address,undefined
H_FILES = ${shell ls */*.hpp}
C_FILES = ${shell ls */*/*.cpp} ${shell ls */*.cpp} 
OBJS    = $(C_FILES:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJS) $(H_FILES)
	@$(CC) $(FLAGS) $(OBJS) -o $(EXEC)
	# @clear
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

.PHONY: all clean fclean re
