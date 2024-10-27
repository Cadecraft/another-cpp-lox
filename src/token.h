#pragma once

#include <string>

#include "structures.h"
#include "loxobject.h"

class Token {
public:
	TokenType type;
	std::string lexeme;
	// NOTE: in the book, this is a Java `Object`; I am using a custom object defined in `loxobject.h`
	LoxObject literal;
	int line;

	// Create the token
	Token(TokenType type, std::string lexeme, LoxObject literal, int line);

	// Stringify the token
	std::string toString();
};
