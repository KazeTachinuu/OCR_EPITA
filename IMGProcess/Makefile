
# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -lm -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image`
all: rotate grayscale

SRC = rotate.c grayscale.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=} 
TMP = *_rotated.bmp

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} ${TMP}



# END
