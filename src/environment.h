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
		values[name] = value;
	}

	// Get the value of a variable
	LoxObject get(Token& name) {
		if (values.find(name.lexeme) != values.end()) {
			// Return a copy
			LoxObject res(values[name.lexeme]);
			return res;
		}
		// Failed
		// TODO: catch and report this error
		throw std::runtime_error("Undefined variable '" + name.lexeme + "'.");
	}

	// Assign a value to a variable
	void assign(Token& name, LoxObject value) {
		if (values.find(name.lexeme) != values.end()) {
			values[name.lexeme] = value; // value is a copy of the LoxObject
			return;
		}
		// Failed
		// TODO: catch and report this error
		throw std::runtime_error("Undefined variable '" + name.lexeme + "'.");
	}
};
