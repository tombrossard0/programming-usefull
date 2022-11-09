# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -fsanitize=address
LDFLAGS = -fsanitize=address
LDLIBS =

SRC = matrix.c user_inputs_functions.c functions.c neural_network.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: 
	$ gcc matrix.c user_inputs_functions.c functions.c neural_network.c -o nn -lm -Wall -Wextra

-include ${DEP}

.PHONY: clean

run:
	nn

# END
