#include "stmt.h"

#include <iostream> // For debugging

// Stmt
// Accept a visitor (see the `visitor pattern`)
std::any Stmt::accept(Visitor* visitor) {
	std::cout << "DBG: the default Stmt accept was called" << std::endl;
	std::string s = "the default Stmt accept in expr.cpp was called";
	return s;
}

// Block
Block::Block(std::vector<Stmt*> statements) : statements(statements) {}
std::any Block::accept(Visitor* visitor) {
	std::cout << "DBG: the Block Stmt accept was called" << std::endl;
	return visitor->visitBlockStmt(*this);
}

// Expression
Expression::Expression(Expr& expr) : expr(expr) {}
std::any Expression::accept(Visitor* visitor) {
	std::cout << "DBG: the Expression Stmt accept was called" << std::endl;
	return visitor->visitExpressionStmt(*this);
}

// Print
Print::Print(Expr& expr) : expr(expr) {}
std::any Print::accept(Visitor* visitor) {
	std::cout << "DBG: the Print Stmt accept was called" << std::endl;
	return visitor->visitPrintStmt(*this);
}

// Var
Var::Var(Token& name, Expr* initializer) : name(name), initializer(initializer) {}
std::any Var::accept(Visitor* visitor) {
	std::cout << "DBG: the Var Stmt accept was called" << std::endl;
	return visitor->visitVarStmt(*this);
}
