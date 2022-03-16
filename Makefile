CC=gcc
CFLAGS=-Wall -Wextra -O3 -flto
LDFLAGS=-flto -L./includes -lagent
INCLUDES=-I./includes

SRC_PATH=./srcs/
SRCS=main.c find_flower.c draw_map.c bee_actions.c
OBJS=main.o find_flower.o draw_map.o bee_actions.o
NAME=agent

all: $(NAME)

$(NAME):
	$(CC) -c $(patsubst %,$(SRC_PATH)%,$(SRCS)) $(CFLAGS) $(INCLUDES)
	$(CC) -o $(NAME) $(OBJS) $(LDFLAGS) $(INCLUDES)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
