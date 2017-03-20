all:
	g++ main.cpp -o main -std=c++14 -Wall -pthread
debug:
	g++ main.cpp -o main -std=c++14 -Wall -g -pthread
checkleak:
	valgrind --leak-check=full --show-leak-kinds=all ./main
test:
	g++ -DTEST main.cpp -o main -std=c++14 -Wall -pthread
	./main < test.txt > results.txt
run:
	./main
