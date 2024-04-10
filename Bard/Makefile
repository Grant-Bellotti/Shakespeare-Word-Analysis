#pasted from previous project in CSE 13s, changed to work with file
CC = gcc
SOURCE = shakespeare.c
OBJ = shakespeare.o
HEADER = shakespeare.h

.PHONY: all clean

all: bard

bard: $(OBJ)
	$(CC) $^ -o $@

%.o: %.c $(HEADER)
	$(CC) -c $< -o $@

clean:
	rm -rf bard
	rm -rf $(OBJ)
