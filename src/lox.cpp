#include <iostream>
#include <sstream>

#include "lox.h"
#include "token.h"
#include "scanner.h"
#include "expr.h"
#include "astprinter.h"
#include "parser.h"
#include "debugprinter.h"

Interpreter Lox::interpreter;

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
	DebugPrinter::print("Debug ast printer output: " + printer.print(root));
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
	} catch (const std::exception& exc) {
		// Failed to read file
		// TODO: remove this debug section?
		std::cerr << exc.what();
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
		DebugPrinter::print("runPrompt STARTING RUNNING");
		run(line);
		DebugPrinter::print("runPrompt FINISHED RUNNING");
		// Do not kill the session in the interactive loop
		hadError = false;
	}
	// Finished
	return 0;
}

void Lox::report(int line, std::string where, std::string message) {
	std::cout << "[line " << line << "] Error" << where << ": " << message << std::endl;
}

// Throw an error
void Lox::error(Token* token, std::string message) {
	if (token->type == TokenType::EndOfFile) {
		report(token->line, " at end", message);
	} else {
		report(token->line, " at '" + token->lexeme + "'", message);
	}
	hadError = true; // Added this line
}

void Lox::error(int line, std::string message) {
	report(line, "", message);
	hadError = true;
}

int Lox::run(std::string s) {
	// Run
	DebugPrinter::print("Running: ");
	DebugPrinter::print(s, 3);

	// Step 1: scan into a list of tokens
	Scanner scanner(s);
	std::vector<Token*> tokens = scanner.scanTokens();

	// DBG: print the tokens
	// TODO: only iterate over/stringify the tokens if we should debug print
	DebugPrinter::print("Tokens:");
	for (auto token : tokens) {
		DebugPrinter::print(token->toString());
	}

	// Step 2: parse
	Parser parser(tokens);
	std::vector<Stmt*> statements;
	try {
		 statements = parser.parse();
	} catch(std::runtime_error& err) {
		// TODO: what to do here?
	}
	// Stop if there was a syntax error
	if (hadError) return 0;

	//Interpreter interpreter;

	// Step 3: run the interpreter
	DebugPrinter::print("Running interpreter on " + std::to_string(statements.size()) + " statements");
	interpreter.interpret(statements);
	DebugPrinter::print("Interpreter finished");

	// TODO: clean up memory

	// Finished
	return 0;
}
