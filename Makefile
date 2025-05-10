# Makefile to compile main.c
CC = clang
CFLAGS = -g -Wall
RM = rm -f

# Default rule main
main: main.c socket.c
	$(CC) $(CFLAGS) -o main main.c socket.c

# Clean rule
clean:
	$(RM) main


