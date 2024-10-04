#include <iostream>
#include <string>
#include <fstream>

// Run the program
class Lox {
public:
	static int runFile(std::string path);

	static int runPrompt();

	static int run(std::string s);
};
