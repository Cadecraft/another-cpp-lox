#pragma once

#include "structures.h"
#include "token.h"
#include "expr.h"
#include "loxobject.h"
#include "lox.h"
#include "stmt.h"

#include <vector>
#include <string>
#include <iostream>
#include <typeinfo>

// TODO: test and implement, or remove
/*class ParseError : public std::runtime_error {
private:
	std::string message;

public:
	// Construct the error
	ParseError(const char* msg) : message(msg) {}

	// Method to return the message
	const char* what() const throw() {
		return message.c_str();
	}
};*/

class Parser {
private:
	std::vector<Token*>& tokens;
	int current = 0;

	// Check if there are no more tokens to parse
	bool isAtEnd() {
		return peek()->type == TokenType::EndOfFile;
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
		return peek()->type == type;
	}

	// Advance: consume the current token and return it
	Token* advance() {
		if (!isAtEnd()) current++;
		return previous();
	}

	// Equality grammar: "matches an equality operator or anything of higher precedence"
	Expr* equality() {
		Expr* expr = comparison();
		while (match(TokenType::BangEqual, TokenType::EqualEqual)) {
			Token* op = previous();
			Expr* right = comparison();
			expr = new Binary(*expr, *op, *right);
		}
		return expr;
	}

	// Comparison grammar: similar to equality
	Expr* comparison() {
		Expr* expr = term();
		while (match(TokenType::Greater, TokenType::GreaterEqual, TokenType::Less, TokenType::LessEqual)) {
			Token* op = previous();
			Expr* right = term();
			expr = new Binary(*expr, *op, *right);
		}
		return expr;
	}

	// Addition and subtractions
	Expr* term() {
		Expr* expr = factor();
		while (match(TokenType::Minus, TokenType::Plus)) {
			Token* op = previous();
			Expr* right = factor();
			expr = new Binary(*expr, *op, *right);
		}
		return expr;
	}

	// Multiplication and division
	Expr* factor() {
		Expr* expr = unary();
		while (match(TokenType::Slash, TokenType::Star)) {
			Token* op = previous();
			Expr* right = unary();
			expr = new Binary(*expr, *op, *right);
		}
		return expr;
	}

	// Unary
	Expr* unary() {
		if (match(TokenType::Bang, TokenType::Minus)) {
			std::cout << "      DBG: parser: matched Bang or Minus" << std::endl;
			Token* op = previous();
			Expr& right = *unary(); // NOTE: corrected from `Expr right` to `Expr& right` due to ownership issues
			std::cout << "      DBG: parser: right expr made, of type " << typeid(right).name() << std::endl;
			return new Unary(*op, right);
		}
		return primary();
		// TODO: (make sure to clean up memory)
	}

	Expr* primary() {
		if (match(TokenType::False)) {
			LoxObject res(0.0); // The book uses true and false, but our LoxObject stores only numbers and strings, no official booleans
			return new Literal(res);
		}
		if (match(TokenType::True)) {
			LoxObject res(1.0);
			return new Literal(res);
		}
		if (match(TokenType::Nil)) {
			LoxObject res;
			return new Literal(res);
		}
		if (match(TokenType::Number, TokenType::String)) {
			return new Literal(previous()->literal);
		}
		if (match(TokenType::LeftParen)) {
			Expr* expr = expression();
			consume(TokenType::RightParen, "Expect ')' after expression.");
			return new Grouping(*expr);
		}
		// No valid expression
		error(peek(), "Expect expression.");
		throw std::runtime_error("See error reported by lox");
	}

	// Expression grammar
	Expr* expression() {
		// Expands to the equality rule (this is a recursive system)
		return equality();
	}
	
	// Consume a specific token
	Token* consume(TokenType type, std::string message) {
		if (check(type)) return advance();
		// Not of the expected type
		error(peek(), message);
		throw std::runtime_error("See error reported by lox");
	}

	// Raise an error upwards
	void error(Token* token, std::string message) {
		Lox::error(token, message);
		// In the book, return a parse error
	}

	// Synchronize the parser if we run into an error
	void synchronize() {
		advance();
		while (!isAtEnd()) {
			if (previous()->type == TokenType::Semicolon) return;
			switch (peek()->type) {
				case TokenType::Class:
				case TokenType::Fun:
				case TokenType::Var:
				case TokenType::For:
				case TokenType::If:
				case TokenType::While:
				case TokenType::Print:
				case TokenType::Return:
					return;
				default:
					break;
			}
		}
		advance();
	}

	Print* printStatement() {
		Expr* value = expression();
		consume(TokenType::Semicolon, "Expect ';' after value.");
		// TODO: use pointer or reference, instead of just Stmt?
		Print* res = new Print(*value);
		return res;
	}

	Expression* expressionStatement() {
		Expr* expr = expression();
		consume(TokenType::Semicolon, "Expect ';' after expression.");
		Expression* res = new Expression(*expr);
		return res;
	}

	Stmt* statement() {
		if (match(TokenType::Print)) {
			Print* res = printStatement();
			return res;
		}
		// Doesn't match any known statement, so assume expression statement
		Expression* res = expressionStatement();
		return res;
	}

public:
	Parser(std::vector<Token*>& tokens) : tokens(tokens) {
		// The tokens are initialized in the header
	}

	// Parse (return null if there were errors)
	std::vector<Stmt*> parse() {
		std::vector<Stmt*> statements;
		while (!isAtEnd()) {
			statements.push_back(statement());
		}
		return statements;

		/*try {
			return expression();
		} catch (const std::runtime_error& e) {
			return nullptr;
		}*/
	}
};
