# Makefile

CC = cc
CPPFLAGS =
CFLAGS =  `pkg-config --libs --cflags SDL2_ttf` -g
LDFLAGS =
LDLIBS = 

all: sudrend 

SRC = sudrend.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=} 
PPM = *.ppm
IMG = *.png

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} ${PPM}
	${RM} ${IMG}

# END
