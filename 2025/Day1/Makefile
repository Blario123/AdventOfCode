CC=gcc
CFLAGS=-g -Wall -Werror -Wpedantic -fsanitize=address

build:
	$(CC) -o Day1 main.c $(CFLAGS)

run: build
	./Day1 input.txt

test: build
	./Day1 example.txt
