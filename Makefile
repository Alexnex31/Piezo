NAME	= piezo

SRC	= src/main.c \
	  src/audio/music.c \
	  src/audio/clock.c

OBJ	= $(SRC:.c=.o)

CFLAGS	+= -Wall -Wextra -Iinclude $(shell pkg-config --cflags sdl3 vorbisfile)
LDLIBS	+= $(shell pkg-config --libs sdl3 vorbisfile) -lm

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $@ $^ $(LDLIBS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re