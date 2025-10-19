all:
	g++ main.cpp -o Day7 -std=c++23

build-debug:
	g++ main.cpp -o Day7 -g -std=c++23

test: all
	./Day7 example.txt

run: all
	./Day7 input.txt

debug: build-debug
	gdb ./Day7
