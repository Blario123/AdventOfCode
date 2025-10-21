all:
	g++ main.cpp -o Day8 -std=c++23

build-debug:
	g++ main.cpp -o Day8 -g -std=c++23

test: all
	./Day8 example.txt

run: all
	./Day8 input.txt

debug: build-debug
	gdb ./Day8
