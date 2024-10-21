#include "loxobject.h"

#include <stdexcept>

#include "structures.h"

LoxObject::LoxObject() {
	t = LoxObjectType::Empty;
}

LoxObject::LoxObject(std::string val) {
	val_s = val;
	t = LoxObjectType::String;
}

LoxObject::LoxObject(int val) {
	val_i = val;
	t = LoxObjectType::Integer;
}

std::string LoxObject::getStringVal() {
	if (t != LoxObjectType::String) {
		throw std::runtime_error("Improper type for LoxObject: expected LoxObject::String");
	} else {
		return val_s;
	}
}

int LoxObject::getIntegerValue() {
	if (t != LoxObjectType::Integer) {
		throw std::runtime_error("Improper type for LoxObject: expected LoxObject::Integer");
	} else {
		return val_i;
	}
}

bool LoxObject::assertEmpty() {
	throw std::runtime_error("Improper type for LoxObject: expected LoxObject::Empty");
}

std::string LoxObject::toString() {
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
