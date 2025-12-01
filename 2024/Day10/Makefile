all:
	g++ main.cpp -o Day10 -std=c++23

build-debug:
	g++ main.cpp -o Day10 -g -std=c++23

test: all
	./Day10 example.txt

run: all
	./Day10 input.txt

debug: build-debug
	gdb ./Day10
