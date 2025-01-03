#pragma once

#include "structures.h"
#include "token.h"
#include "expr.h"
#include "loxobject.h"
#include "lox.h"
#include "stmt.h"
#include "debugprinter.h"

#include <vector>
#include <string>
#include <iostream>
#include <typeinfo>
#include <memory>

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
	std::vector<std::shared_ptr<Token>>& tokens;
	int current = 0;

	// Check if there are no more tokens to parse
	bool isAtEnd() {
		return peek()->type == TokenType::EndOfFile;
	}

	// Check the current token (not yet consumed)
	std::shared_ptr<Token> peek() {
		return tokens[current];
	}

	// Return the most recently consumed token
	std::shared_ptr<Token> previous() {
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
	std::shared_ptr<Token> advance() {
		if (!isAtEnd()) current++;
		return previous();
	}

	// Equality grammar: "matches an equality operator or anything of higher precedence"
	Expr* equality() {
		Expr* expr = comparison();
		while (match(TokenType::BangEqual, TokenType::EqualEqual)) {
			std::shared_ptr<Token> op = previous();
			Expr* right = comparison();
			expr = new Binary(*expr, *op, *right);
		}
		return expr;
	}

	// Comparison grammar: similar to equality
	Expr* comparison() {
		Expr* expr = term();
		while (match(TokenType::Greater, TokenType::GreaterEqual, TokenType::Less, TokenType::LessEqual)) {
			std::shared_ptr<Token> op = previous();
			Expr* right = term();
			expr = new Binary(*expr, *op, *right);
		}
		return expr;
	}

	// Addition and subtractions
	Expr* term() {
		Expr* expr = factor();
		while (match(TokenType::Minus, TokenType::Plus)) {
			std::shared_ptr<Token> op = previous();
			Expr* right = factor();
			expr = new Binary(*expr, *op, *right);
		}
		return expr;
	}

	// Multiplication and division
	Expr* factor() {
		Expr* expr = unary();
		while (match(TokenType::Slash, TokenType::Star)) {
			std::shared_ptr<Token> op = previous();
			Expr* right = unary();
			expr = new Binary(*expr, *op, *right);
		}
		return expr;
	}

	// Unary
	Expr* unary() {
		if (match(TokenType::Bang, TokenType::Minus)) {
			//std::cout << "      DBG: parser: matched Bang or Minus" << std::endl;
			DebugPrinter::print("parser: matched Bang or Minus");
			std::shared_ptr<Token> op = previous();
			Expr& right = *unary(); // NOTE: corrected from `Expr right` to `Expr& right` due to ownership issues
			//std::cout << "      DBG: parser: right expr made, of type " << typeid(right).name() << std::endl;
			DebugPrinter::print("parser: right expr made, of type: " + DebugPrinter::to_string(typeid(right).name()));
			return new Unary(*op, right);
		}
		return primary();
		// TODO: (make sure to clean up memory)
	}

	Expr* primary() {
		if (match(TokenType::False)) {
			LoxObject* res = new LoxObject(0.0); // The book uses true and false, but our LoxObject stores only numbers and strings, no official booleans
			return new Literal(*res);
		}
		if (match(TokenType::True)) {
			LoxObject* res = new LoxObject(1.0);
			return new Literal(*res);
		}
		if (match(TokenType::Nil)) {
			LoxObject* res = new LoxObject();
			return new Literal(*res);
		}
		if (match(TokenType::Number, TokenType::String)) {
			return new Literal(previous()->literal);
		}
		if (match(TokenType::Identifier)) {
			return new Variable(*previous());
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

	// And
	Expr* and_expr() {
		Expr* expr = equality();

		while (match(TokenType::And)) {
			std::shared_ptr<Token> op = previous();
			Expr* right = equality();
			expr = new Logical(*expr, *op, *right);
		}

		return expr;
	}

	// Or
	Expr* or_expr() {
		Expr* expr = and_expr();

		while (match(TokenType::Or)) {
			std::shared_ptr<Token> op = previous();
			Expr* right = and_expr();
			expr = new Logical(*expr, *op, *right);
		}

		return expr;
	}

	// Assignment
	Expr* assignment() {
		Expr* expr = or_expr();

		if (match(TokenType::Equal)) {
			std::shared_ptr<Token> equals = previous();
			// Recursion
			Expr* value = assignment();
			// LSP error: "Expr is not polymorphic" (should not be an issue, just because of std::any)
			if (dynamic_cast<Variable*>(expr) != nullptr) {
				// Is a variable
				//std::shared_ptr<Token> name = &(dynamic_cast<Variable*>(expr)->name);
				// TODO: memory issues here? (eventually make the name in Variable a shared ptr
				// TODO: simply copy the actual value of the name?
				Token name(dynamic_cast<Variable*>(expr)->name);
				return new Assign(name, *value);
			}
			error(equals, "Invalid assignment target.");
		}

		return expr;
	}

	// Expression grammar
	Expr* expression() {
		// Expands to the assignment, then equality rule (this is a recursive system)
		return assignment();
	}

	// Consume a specific token
	std::shared_ptr<Token> consume(TokenType type, std::string message) {
		if (check(type)) return advance();
		// Not of the expected type
		error(peek(), message);
		throw std::runtime_error("See error reported by lox");
	}

	// Raise an error upwards
	void error(std::shared_ptr<Token> token, std::string message) {
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
		//std::cout << "  DBG: print statement expression is: " << typeid(value).name() << std::endl; // TODO: remove
		DebugPrinter::print("print statement expression is: " + DebugPrinter::to_string(typeid(value).name()));
		return new Print(*value);
	}

	Expression* expressionStatement() {
		Expr* expr = expression();
		consume(TokenType::Semicolon, "Expect ';' after expression.");
		return new Expression(*expr);
	}

	std::vector<Stmt*> block() {
		std::vector<Stmt*> statements;
		while (!check(TokenType::RightBrace) && !isAtEnd()) {
			statements.push_back(declaration());
		}
		consume(TokenType::RightBrace, "Expect '}' after block.");
		return statements;
	}

	If* ifStatement() {
		// Get the condition
		consume(TokenType::LeftParen, "Expect '(' after 'if'.");
		Expr* condition = expression();
		consume(TokenType::RightParen, "Expect ')' after if condition.");
		// Get the branches
		Stmt* thenBranch = statement();
		Stmt* elseBranch = nullptr;
		if (match(TokenType::Else)) {
			elseBranch = statement();
		}
		return new If(*condition, *thenBranch, elseBranch);
	}

	While* whileStatement() {
		consume(TokenType::LeftParen, "Expect '(' after 'while'.");
		Expr* condition = expression();
		consume(TokenType::RightParen, "Expect ')' after condition.");
		Stmt* body = statement();
		return new While(*condition, *body);
	}

	Stmt* statement() {
		if (match(TokenType::If)) {
			If* res = ifStatement();
			return res;
		}
		if (match(TokenType::Print)) {
			Print* res = printStatement();
			return res;
		}
		if (match(TokenType::While)) {
			While* res = whileStatement();
			return res;
		}
		if (match(TokenType::LeftBrace)) {
			return new Block(block());
		}
		// Doesn't match any known statement, so assume expression statement
		Expression* res = expressionStatement();
		return res;
	}

	Stmt* varDeclaration() {
		std::shared_ptr<Token> name = consume(TokenType::Identifier, "Expect variable name.");
		Expr* initializer = nullptr;
		if (match(TokenType::Equal)) {
			initializer = expression();
		}
		consume(TokenType::Semicolon, "Expect ';' after variable declaration.");
		return new Var(*name, initializer);
	}

	Stmt* declaration() {
		try {
			if (match(TokenType::Var)) return varDeclaration();
			// Doesn't match a variable, so assume other statement
			return statement();
		} catch (std::runtime_error& err) {
			// TODO: instead of runtime_error, use ParseError??
			synchronize();
			// TODO: does returning nullptr make sense here?
			return nullptr;
		}
	}

public:
	Parser(std::vector<std::shared_ptr<Token>>& tokens) : tokens(tokens) {
		// The tokens are initialized in the header
	}

	// Parse (return null if there were errors)
	std::vector<Stmt*> parse() {
		std::vector<Stmt*> statements;
		while (!isAtEnd()) {
			statements.push_back(declaration());
		}
		return statements;

		// TODO: remove this old code
		/*try {
			return expression();
		} catch (const std::runtime_error& e) {
			return nullptr;
		}*/
	}
};
