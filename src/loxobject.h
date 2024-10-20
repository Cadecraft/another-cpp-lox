#include "structures.h"

// A lox object
class LoxObject {
private:
	std::string val_s;
	int val_i;
	LoxObjectType t;

public:
	// Construct a lox object of the empty type
	LoxObject() {
		t = LoxObjectType::Empty;
	}

	// Construct a lox object of type String
	LoxObject(std::string val) {
		val_s = val;
		t = LoxObjectType::String;
	}

	// Construct a lox object of type Integer
	LoxObject(int val) {
		val_i = val;
		t = LoxObjectType::Integer;
	}

	// Obtain the value of the lox object of type String
	std::string getStringVal() {
		if (t != LoxObjectType::String) {
			throw std::runtime_error("Improper type for LoxObject: expected LoxObject::String");
		} else {
			return val_s;
		}
	}

	// Obtain the value of the lox object of type Integer
	int getIntegerValue() {
		if (t != LoxObjectType::Integer) {
			throw std::runtime_error("Improper type for LoxObject: expected LoxObject::Integer");
		} else {
			return val_i;
		}
	}
	
	// Assert that the lox object is of type Empty
	bool assertEmpty() {
		throw std::runtime_error("Improper type for LoxObject: expected LoxObject::Empty");
	}

	// Get the string representation
	std::string toString() {
		switch (t) {
		case LoxObjectType::String:
			return val_s;
		case LoxObjectType::Integer:
			return std::to_string(val_i);
		case LoxObjectType::Empty:
			return "(Empty)";
		default:
			return "(No valid type)";
		}
	}
};
