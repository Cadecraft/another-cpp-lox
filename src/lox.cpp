#include <iostream>
#include <sstream>

#include "lox.h"
#include "token.h"
#include "scanner.h"
#include "expr.h"
#include "astprinter.h"

bool Lox::hadError = false;

void Lox::debug() {
	// Debug
	LoxObject empty;
	Token token(TokenType::Minus, "-", empty, 1);
	LoxObject ott(123);
	LoxObject ffss(45.67);
	Literal literal(ott);
	Unary unary(token, literal);
	Token star(TokenType::Star, "*", empty, 1);
	Literal other(ffss);
	Grouping grouping(other);
	Binary root(unary, star, grouping);
	AstPrinter printer;
	//std::cout << printer.print(root) << std::endl;
}

int Lox::runFile(std::string path) {
	// Read all bytes
	std::ifstream f;
	try {
		f.open(path);
		std::stringstream buffer;
		buffer << f.rdbuf();
		run(buffer.str());
		if (hadError) return 65;
		else return 0;
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
		// Do not kill the session in the interactive loop
		hadError = false;
	}
	// Finished
	return 0;
}

void Lox::report(int line, std::string where, std::string message) {
	std::cout << "[line " << line << "] Error" << where << ": " << message << std::endl;
}

void Lox::error(int line, std::string message) {
	report(line, "", message);
	hadError = true;
}

int Lox::run(std::string s) {
	// Run
	std::cout << "DBG: Running: " << std::endl;
	std::cout << s << std::endl;

	// Step 1: scan into a list of tokens
	Scanner scanner(s);
	std::vector<Token*> tokens = scanner.scanTokens();

	// DBG: print the tokens
	for (auto token : tokens) {
		std::cout << token->toString() << ",";
	}
	std::cout << std::endl;

	// TODO: clean up memory

	// Finished
	return 0;
}
