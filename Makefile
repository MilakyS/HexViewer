TARGET = hexview

CC = gcc
CFLAGS = -Wall -Wextra -std=c23

SRC = main.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)
clean:
	rm -f $(TARGET)