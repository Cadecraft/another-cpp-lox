#include <string>

#include "structures.h"
#include "loxobject.h"

class Token {
private:
	TokenType type;
	std::string lexeme;
	// NOTE: in the book, this is a Java `Object`; I am using a custom object defined in `loxobject.h`
	LoxObject literal;
	int line;

public:
	// Create the token
	Token(TokenType type, std::string lexeme, LoxObject literal, int line) {
		this->type = type;
		this->lexeme = lexeme;
		this->literal = literal;
		this->line = line;
	}

	// Stringify the token
	std::string toString() {
		// Cannot display the type
		// TODO: fix
		return lexeme + " " + literal.toString();
	}
};
