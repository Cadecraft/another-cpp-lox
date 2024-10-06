#include <string>

#include "structures.h"

class Token {
private:
	TokenType type;
	std::string lexeme;
	int literal; // NOTE: in the book, this is a Java `Object`
	int line;

public:
	// Create the token
	Token(TokenType type, std::string lexeme, int literal, int line) {
		this->type = type;
		this->lexeme = lexeme;
		this->literal = literal;
		this->line = line;
	}

	// Stringify the token
	std::string toString() {
		// Cannot display the type
		// TODO: fix
		return lexeme + " " + std::to_string(literal);
	}
};
