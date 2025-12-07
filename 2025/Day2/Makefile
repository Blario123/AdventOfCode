CC=gcc
CFLAGS=-g -Wall -Werror -Wpedantic -fsanitize=address

build:
	$(CC) -o Day2 main.c $(CFLAGS)

run: build
	./Day2 input.txt

test: build
	./Day2 example.txt
