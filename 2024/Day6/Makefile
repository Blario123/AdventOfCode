all:
	g++ main.cpp -o Day6

debug-build:
	g++ main.cpp -DDEBUG -o Day6

debug-test: debug-build
	./Day6 example.txt

debug-run: debug-build
	./Day6 input.txt

gif-build:
	g++ main.cpp -DGIF --std=c++20 -o Day6

gif-test: gif-build
	./Day6 example.txt

gif-run: gif-build
	./Day6 input.txt

test: all
	./Day6 example.txt

debug:
	g++ -g main.cpp -o Day6

run: all
	./Day6 input.txt
