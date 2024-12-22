# Nom de l’exécutable
NAME = philo

# Compilateur
CC = cc

# Flags de compilation
CFLAGS = -Wall -Wextra -Werror -pthread

# Sources et objets
SRCS = philo.c philosopher.c utils.c
OBJS = $(SRCS:.c=.o)

# Règles
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
