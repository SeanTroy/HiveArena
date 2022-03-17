CC=gcc
CFLAGS=-Wall -Wextra -O3 -flto
LDFLAGS=-flto -L./libagent_src -lagent
INCLUDES=-I./includes
LIBAGENT_PATH=./libagent_src/

SRC_PATH=./srcs/
SRCS=main.c find_flower.c draw_map.c bee_actions.c
OBJS=main.o find_flower.o draw_map.o bee_actions.o
NAME=agent

all: $(NAME)

$(NAME):
	make -C $(LIBAGENT_PATH)
	$(CC) -c $(patsubst %,$(SRC_PATH)%,$(SRCS)) $(CFLAGS) $(INCLUDES)
	$(CC) -o $(NAME) $(OBJS) $(LDFLAGS) $(INCLUDES)

clean:
	rm -f $(OBJS)
	make clean -C $(LIBAGENT_PATH)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBAGENT_PATH)

re: fclean all
