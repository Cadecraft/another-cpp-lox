all:
	g++ -g -Wall -o anothercpplox src/main.cpp src/interpreter.cpp src/lox.cpp src/loxobject.cpp src/token.cpp src/scanner.cpp src/expr.cpp src/stmt.cpp
