# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra
LDFLAGS =
LDLIBS =

SRC = solver.c 
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
RESULT = *.result
solver: ${OBJ}

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} ${RESULT}
	${RM} solver 

# END
