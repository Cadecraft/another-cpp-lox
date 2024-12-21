#include "stmt.h"

// Stmt
// Accept a visitor (see the `visitor pattern`)
std::any Stmt::accept(Visitor* visitor) {
	//return visitor.visitBinaryExpr(*this);
	std::string s = "the default Stmt accept in expr.cpp was called";
	return s;
}

// Expression
Expression::Expression(Expr& expr) : expr(expr) {}
std::any Expression::accept(Visitor* visitor) {
	/*std::string s = std::any_cast<std::string>(visitor->visitBinaryExpr(*this));
	return s;*/
	return visitor->visitExpressionStmt(*this);
}

// Print
Print::Print(Expr& expr) : expr(expr) {}
std::any Print::accept(Visitor* visitor) {
	/*std::string s = std::any_cast<std::string>(visitor->visitBinaryExpr(*this));
	return s;*/
	return visitor->visitPrintStmt(*this);
}
