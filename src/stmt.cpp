#include "stmt.h"
#include "debugprinter.h"

// Stmt
// Accept a visitor (see the `visitor pattern`)
std::any Stmt::accept(Visitor* visitor) {
	DebugPrinter::print("the default Stmt accept was called");
	std::string s = "the default Stmt accept in expr.cpp was called";
	return s;
}

// Block
Block::Block(std::vector<Stmt*> statements) : statements(statements) {}
std::any Block::accept(Visitor* visitor) {
	DebugPrinter::print("the Block Stmt accept was called");
	return visitor->visitBlockStmt(*this);
}

// Expression
Expression::Expression(Expr& expr) : expr(expr) {}
std::any Expression::accept(Visitor* visitor) {
	DebugPrinter::print("the Expression Stmt accept was called");
	return visitor->visitExpressionStmt(*this);
}

// If
If::If(Expr& condition, Stmt& thenBranch, Stmt* elseBranch) : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}
std::any If::accept(Visitor* visitor) {
	DebugPrinter::print("the If Stmt accept was called");
	return visitor->visitIfStmt(*this);
}

// Print
Print::Print(Expr& expr) : expr(expr) {}
std::any Print::accept(Visitor* visitor) {
	DebugPrinter::print("the Print Stmt accept was called");
	return visitor->visitPrintStmt(*this);
}

// Var
Var::Var(Token& name, Expr* initializer) : name(name), initializer(initializer) {}
std::any Var::accept(Visitor* visitor) {
	DebugPrinter::print("the Var Stmt accept was called");
	return visitor->visitVarStmt(*this);
}
