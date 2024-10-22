#include <string>

#include "token.h"

Token::Token(TokenType type, std::string lexeme, LoxObject literal, int line) {
	this->type = type;
	this->lexeme = lexeme;
	this->literal = literal;
	this->line = line;
}

std::string Token::toString() {
	// Cannot display the type
	// TODO: fix
	return lexeme + " " + literal.toString();
}
