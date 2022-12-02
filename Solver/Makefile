# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra
LDFLAGS =
LDLIBS =

SRC = solver.c generate.c main.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
RESULT = *.result
GEN = *.gen
all : solver generate 


-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} ${RESULT}
	${RM} ${GEN}
	${RM} solver
	${RM} generate

# END
