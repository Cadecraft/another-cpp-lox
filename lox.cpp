#include <iostream>
#include <sstream>

#include "lox.h"

int Lox::runFile(std::string path) {
	// Read all bytes
	std::ifstream f;
	try {
		f.open(path);
		std::stringstream buffer;
		buffer << f.rdbuf();
		return run(buffer.str());
	} catch (...) {
		// Failed to read file
		return 1;
	}
}

int Lox::runPrompt() {
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
	// Finished
	return 0;
}

int Lox::run(std::string s) {
	// Run
	std::cout << "DBG: Running: " << std::endl;
	std::cout << s << std::endl;
	// Finished
	return 0;
}
