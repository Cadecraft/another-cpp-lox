#pragma once

#include "structures.h"

// A lox object
class LoxObject {
private:
	std::string val_s;
	int val_i;
	LoxObjectType t;

public:
	// Construct a lox object of the empty type
	LoxObject();

	// Construct a lox object of type String
	LoxObject(std::string val);

	// Construct a lox object of type Integer
	LoxObject(int val);

	// Obtain the value of the lox object of type String
	std::string getStringVal();

	// Obtain the value of the lox object of type Integer
	int getIntegerValue();
	
	// Assert that the lox object is of type Empty
	bool assertEmpty();

	// Get the string representation
	std::string toString();
};
