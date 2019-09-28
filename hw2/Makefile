all: basic_interpreter

basic_interpreter: interpreter/maintest.cpp interpreter/arithmetic.o interpreter/boolean.o interpreter/command.o interpreter/interpreter.o
	g++ -g -Wall main.cpp interpreter/arithmetic.o interpreter/boolean.o interpreter/command.o interpreter/interpreter.o -o basic_interpreter

interpreter/arithmetic.o: interpreter/arithmetic.h interpreter/arithmetic.cpp
	g++ -g -Wall -c interpreter/arithmetic.cpp -o interpreter/arithmetic.o

interpreter/boolean.o: interpreter/boolean.h interpreter/boolean.cpp
	g++ -g -Wall -c interpreter/boolean.cpp -o interpreter/boolean.o

interpreter/command.o: interpreter/command.h interpreter/command.cpp
	g++ -g -Wall -c interpreter/command.cpp -o interpreter/command.o

interpreter/interpreter.o: interpreter/interpreter.h interpreter/interpreter.cpp
	g++ -g -Wall -c interpreter/interpreter.cpp -o interpreter/interpreter.o