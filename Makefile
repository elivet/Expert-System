NAME = expert

FLAGS = -Wall -Wextra -Werror

OBJ = main.o Expert.o Connection.o Link.o And.o Or.o Query.o Xor.o Rule.o

all: $(NAME)

$(NAME): $(OBJ)
	    g++ $(FLAGS) -o $(NAME) $(OBJ)

%.o: %.cpp
	    g++ $(FLAGS) -c $<

clean:
	    rm -rf $(OBJ)

fclean: clean
	    rm -rf $(NAME)

re: fclean all