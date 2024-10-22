all:
	g++ -g -Wall -o anothercpplox src/main.cpp src/lox.cpp src/loxobject.cpp src/token.cpp src/scanner.cpp
