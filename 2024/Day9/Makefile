all:
	g++ main.cpp -o Day9 -std=c++23

build-debug:
	g++ main.cpp -o Day9 -g -std=c++23

test: all
	./Day9 example.txt

run: all
	./Day9 input.txt

debug: build-debug
	gdb ./Day9
