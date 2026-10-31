NAME	= piezo

SRC	= src/main.c \
	  src/core/app.c \
	  src/core/options.c \
	  src/core/input.c \
	  src/audio/music.c \
	  src/audio/clock.c \
	  src/game/metronome.c \
	  src/game/chart.c \
	  src/render/render.c \
	  src/render/flash.c \
	  src/render/hud.c \
	  src/render/playfield.c \
	  src/render/shapes.c

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