#pragma once

#include "token.h"

// TODO: test and implement, or remove
class RuntimeError : public std::runtime_error {
public:
	Token& token;
	std::string message;

	// Construct the error
	RuntimeError(Token& token, const char* msg) : std::runtime_error(msg), token(token), message(msg) {}

	// Method to return the message
	const char* what() const throw() {
		return message.c_str();
	}
};
