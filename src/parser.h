#pragma once

#include "structures.h"
#include "token.h"
#include "expr.h"

#include <vector>

class Parser {
private:
	std::vector<Token*>& tokens;
	int current = 0;

	// Check if there are no more tokens to parse
	bool isAtEnd() {
		return peek().type == TokenType::EndOfFile;
	}

	// Check the current token (not yet consumed)
	Token* peek() {
		return tokens[current];
	}

	// Return the most recently consumed token
	Token* previous() {
		return tokens[current - 1];
	}

	// Match: check if any of the given types match
	bool match(TokenType a) {
		if (check(a)) {
			advance();
			return true;
		}
		return false;
	}

	// Match: check if any of the given types match
	bool match(TokenType a, TokenType b) {
		if (check(a)) {
			advance();
			return true;
		}
		if (check(b)) {
			advance();
			return true;
		}
		return false;
	}

	// Match: check if any of the given types match
	bool match(TokenType a, TokenType b, TokenType c) {
		if (check(a)) {
			advance();
			return true;
		}
		if (check(b)) {
			advance();
			return true;
		}
		if (check(c)) {
			advance();
			return true;
		}
		return false;
	}

	// Match: check if any of the given types match
	bool match(TokenType a, TokenType b, TokenType c, TokenType d) {
		if (check(a)) {
			advance();
			return true;
		}
		if (check(b)) {
			advance();
			return true;
		}
		if (check(c)) {
			advance();
			return true;
		}
		if (check(d)) {
			advance();
			return true;
		}
		return false;
	}

	// Check: return whether the current token is of the given type without consuming the token
	bool check(TokenType type) {
		if (isAtEnd()) return false;
		return peek().type == type;
	}

	// Advance: consume the current token and return it
	Token* advance() {
		if (!isAtEnd()) current++;
		return previous();
	}

	// Equality grammar: "matches an equality operator or anything of higher precedence"
	Expr equality() {
		Expr expr = comparison();
		while (match(TokenType::BangEqual, TokenType::EqualEqual)) {
			Token operator = previous();
			Expr right = comparison();
			expr = new Expr.Binary(expr, operator, right);
		}
		return expr;
	}

	// Comparison grammar: similar to equality
	Expr comparison() {
		Expr expr = comparison();
		while (match(TokenType::Greater, TokenType::GreaterEqual, TokenType::Less, TokenType::LessEqual)) {
			Token operator = previous();
			Expr right = term();
			expr = new Expr.Binary(expr, operator, right);
		}
		return expr;
	}

	// Addition and subtractions
	Expr term() {
		Expr expr = factor();
		while (match(TokenType::Minus, TokenType::Plus)) {
			Token operator = previous();
			Expr right = factor();
			expr = new Binary(expr, operator, right); // TODO: check this works
		}
		return expr;
	}

	// Multiplication and division
	Expr factor() {
		Expr expr = unary();
		while (match(TokenType::Slash, TokenType::Star)) {
			Token operator = previous();
			Expr right = unary();
			expr = new Binary(expr, operator, right);
		}
		return expr;
	}

	// Expression grammar
	Expr expression() {
		// Expands to the equality rule (this is a recursive system)
		return equality();
	}

public:
	Parser(std::vector<Token*>& tokens) : tokens(tokens) {
		// TODO: ensure tokens can be passed as reference (not mutated)
	}
};
