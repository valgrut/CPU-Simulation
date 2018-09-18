CC=g++
FLAGS=-std=c++14 -Wall -Wextra

main: main.cpp
	$(CC) $(FLAGS) main.cpp -o main
