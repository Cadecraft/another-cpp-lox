#include "loxobject.h"
#include "token.h"

#include <iostream>
#include <string>
#include <map>

class Environment {
private:
	std::map<std::string, LoxObject> values;

public:
	// Define a new variable (also allows a variable to be redefined)
	void define(std::string name, LoxObject value) {
		values[name] = value; // TODO: copying? referencing?
	}

	// Get the value of a variable
	LoxObject get(Token& name) {
		// TODO: are the lexemes always correct?
		if (values.find(name.lexeme) != values.end()) {
			// Return a copy
			LoxObject res(values[name.lexeme]);
			return res;
		}
		// Failed
		throw std::runtime_error("Undefined variable '" + name.lexeme + "'.");
	}
};
