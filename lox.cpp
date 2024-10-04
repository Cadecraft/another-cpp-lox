#include <iostream>

#include "lox.h"

static int Lox::runFile(std::string path) {
	// Read all bytes
	std::ifstream f;
	try {
		f.open(path);
		std::string contents;
		f >> contents;
		run(contents);
	} catch {
		// Failed to read file
		return 1;
	}
}

static int Lox::runPrompt() {
	// Read bytes as they are inputted
	std::cout << "Enter code line by line (enter an empty line to exit):" << std::endl;
	// REPL: Read a line of input, Evaluate it, Print result, and Loop again
	while (true) {
		std::string line;
		std::getline(std::cin, line);
		if (line.empty()) {
			// User exited
			return 0;
		}
		run(line);
	}
	return 0;
}

static int Lox::run(std::string s) {

}
