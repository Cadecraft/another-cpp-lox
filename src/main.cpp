#include <iostream>
#include <string>

#include "lox.h"

// Entry point for the program
int main(int argc, const char* argv[]) {
	// DEBUG
	Lox::debug();
	// Handle the arguments
	if (argc > 2) {
		// User doesn't know how to use the program
		std::cout << "Usage: `cpplox {script}`" << std::endl;
		return 0;
	} else if (argc == 2) {
		// Run the file
		std::string arg1(argv[1]);
		Lox::runFile(arg1);
	} else {
		// Interactive prompt for script
		Lox::runPrompt();
	}
}
