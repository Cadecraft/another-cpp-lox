#include "expr.h"
#include <string>
#include <iostream> // For debugging

// Visitor
// TODO: template necessary?
std::any Visitor::visitBinaryExpr(Binary& expr) {
	// Do stuff
	return "the default Visitor visit in expr.cpp was called";
}
std::any Visitor::visitGroupingExpr(Grouping& expr) {
	// Do stuff
	return "the default Visitor visit in expr.cpp was called";
}
std::any Visitor::visitLiteralExpr(Literal& expr) {
	// Do stuff
	return "the default Visitor visit in expr.cpp was called";
}
std::any Visitor::visitUnaryExpr(Unary& expr) {
	// Do stuff
	return "the default Visitor visit in expr.cpp was called";
}

// Expr
// Accept a visitor (see the `visitor pattern`)
std::any Expr::accept(Visitor* visitor) {
	//return visitor.visitBinaryExpr(*this);
	std::string s = "the default Expr accept in expr.cpp was called";
	return s;
}

// Binary
Binary::Binary(Expr& left, Token& op, Expr& right) : left(left), op(op), right(right) { }
// TODO: override with virtual (replace the types with a LoxObject?
// TODO: implement accept for all
std::any Binary::accept(Visitor* visitor) {
	/*std::string s = std::any_cast<std::string>(visitor->visitBinaryExpr(*this));
	return s;*/
	return visitor->visitBinaryExpr(*this);
}

// Grouping
Grouping::Grouping(Expr& expression) : expression(expression) { }
std::any Grouping::accept(Visitor* visitor) {
	/*std::string s = std::any_cast<std::string>(visitor->visitGroupingExpr(*this));
	return s;*/
	return visitor->visitGroupingExpr(*this);
}

// Literal
Literal::Literal(LoxObject& obj) : obj(obj) { }
std::any Literal::accept(Visitor* visitor) {
	/*std::string s = std::any_cast<std::string>(visitor->visitLiteralExpr(*this));
	return s;*/
	return visitor->visitLiteralExpr(*this);
}

// Unary
Unary::Unary(Token& op, Expr& right) : op(op), right(right) { }
std::any Unary::accept(Visitor* visitor) {
	/*std::string s = std::any_cast<std::string>(visitor->visitUnaryExpr(*this));
	return s;*/
	return visitor->visitUnaryExpr(*this);
}
