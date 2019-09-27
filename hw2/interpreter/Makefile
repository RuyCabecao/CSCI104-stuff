all: test

test: maintest.cpp arithmetic.o boolean.o command.o interpreter.o
	g++ -g -Wall maintest.cpp arithmetic.o boolean.o command.o interpreter.o -o test

arithmetic.o: arithmetic.h arithmetic.cpp
	g++ -g -Wall -c arithmetic.cpp -o arithmetic.o

boolean.o: boolean.h boolean.cpp
	g++ -g -Wall -c boolean.cpp -o boolean.o

command.o: command.h command.cpp
	g++ -g -Wall -c command.cpp -o command.o

interpreter.o: interpreter.h interpreter.cpp
	g++ -g -Wall -c interpreter.cpp -o interpreter.o