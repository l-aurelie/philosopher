NAME    =    philo
SRC     =    philo.c lib_utils.c philo_utils.c init.c simulation.c
OBJ     =    $(SRC:.c=.o)
CC      =    gcc
CFLAGS  =    -Wall -Wextra -Werror
HEADER  =    -I.

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(HEADER) -o $@ $(OBJ) -pthread

%.o: %.c
	$(CC) $(CFLAGS) $(HEADER) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY:	all clean fclean re
