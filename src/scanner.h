#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <memory>

#include "structures.h"
#include "token.h"
#include "lox.h"

class Scanner {
private:
	std::string source;
	std::vector<std::shared_ptr<Token>> tokens;
	std::map<std::string, TokenType> keywords;

	int start;
	int current;
	int line;
	
	// Return whether we have consumed all the characters
	bool isAtEnd();

	// Return the current then advance ahead
	char advance();

	// Add a token of a specific type
	void addToken(TokenType type);

	// Add a token
	// Literal is allowed to be a string, int, or other object (TODO: refactor)
	void addToken(TokenType type, LoxObject literal);

	// Check if match, and if so, consume the current character (like conditional version of `advance()`)
	bool match(char expected);

	// Advance without consuming the character (technically called "lookahead")
	char peek();

	// Advance twice without consuming the characters
	char peekNext();

	// String
	void string();

	// Check whether a character is a digit
	bool isDigit(char c);

	// Check whether a character is an alpha character
	bool isAlpha(char c);

	// Check whether a number is an alpha character or a digit
	bool isAlphaNumeric(char c);

	// Number
	void number();

	// Identifier
	void identifier();

	// Scan an individual token
	void scanToken();

public:
	// Create the scanner with a source string
	Scanner(std::string source);

	// Scan the tokens
	std::vector<std::shared_ptr<Token>> scanTokens();
};
