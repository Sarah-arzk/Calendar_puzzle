CC = gcc
CFLAGS = -Wall -Wextra -O2 -pthread

NAME = puzzle

SRC = main.c solver.c global.c
OBJ = $(SRC:.c=.o)

#  COMPILATION PRINCIPALE 

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

# règle générique : compile chaque .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

#  CLEAN 

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
