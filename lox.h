#include <iostream>
#include <string>
#include <fstream>

// Provide the various entry points to the interpreter
class Lox {
public:
	// Open and run a lox program as a file
	static int runFile(std::string path);

	// Run an interactive prompt for the lox interpreter
	static int runPrompt();

	// Run a lox program
	static int run(std::string s);
};
