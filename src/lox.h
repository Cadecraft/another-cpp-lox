#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <memory>

#include "token.h"
#include "interpreter.h"

// Provide the various entry points to the interpreter
class Lox {
private:
	static Interpreter interpreter;

	// Whether the program ran had an error
	static bool hadError;

	// Report an error
	static void report(int line, std::string where, std::string message);

public:
	// Run a debug sequence
	static void debug();

	// Open and run a lox program as a file
	static int runFile(std::string path);

	// Run an interactive prompt for the lox interpreter
	static int runPrompt();

	// Experience an error
	static void error(int line, std::string message);

	// Experience an error with tokens (safe)
	static void error(std::shared_ptr<Token> token, std::string message);

	// Experience an error with tokens
	static void error(Token* token, std::string message);

	// Run a lox program
	static int run(std::string s);
};
