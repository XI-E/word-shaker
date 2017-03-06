all:
	g++ main.cpp -o main -std=c++14 -Wall
debug:
	g++ main -o main -std=c++14 -Wall -g
checkleak:
	valgrind --leak-check=full --show-leak-kinds=all ./main
test:
	g++ -DTEST main.cpp -o main -std=c++14 -Wall
	./main < test.txt > results.txt
run:
	./main
