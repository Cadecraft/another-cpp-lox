#pragma once

#include <iostream>
#include <string>

// Print based on whether the debug message mode is set

const bool DEBUG_MESSAGES = false; // Set to true to see debug messages

class DebugPrinter {
public:
	// Convert a `const char*` to a `std::string`
	static std::string to_string(const char* c) {
		std::string s(c);
		return s;
	}

	// Print a message at a certain nested level (2 spaces per level)
	static bool print(std::string message, int level = 2) {
		if (DEBUG_MESSAGES) {
			for (int i = 0; i < level; i++) {
				std::cout << "  ";
			}
			std::cout << "DBG: " << message << std::endl;
			return true;
		} else {
			return false;
		}
	}
};
