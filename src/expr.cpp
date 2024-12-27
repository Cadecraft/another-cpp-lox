#include "expr.h"
#include <string>
#include <iostream> // For debugging

// Visitor visit default definitions
// (These should never be actually used, but are necessary for compilation)
std::any Visitor::visitBinaryExpr(Binary& expr) {
	return "the default Visitor visit in expr.cpp was called (binary)";
}
std::any Visitor::visitGroupingExpr(Grouping& expr) {
	return "the default Visitor visit in expr.cpp was called (grouping)";
}
std::any Visitor::visitLiteralExpr(Literal& expr) {
	return "the default Visitor visit in expr.cpp was called (literal)";
}
std::any Visitor::visitUnaryExpr(Unary& expr) {
	return "the default Visitor visit in expr.cpp was called (unary)";
}
std::any Visitor::visitVariableExpr(Variable& expr) {
	return "the default Visitor visit in expr.cpp was called (variable)";
}
std::any Visitor::visitAssignExpr(Assign& expr) {
	return "the default Visitor visit in expr.cpp was called (assign)";
}
std::any Visitor::visitBlockStmt(Block& stmt) {
	return "the default Visitor visit in expr.cpp was called (block stmt)";
}
std::any Visitor::visitExpressionStmt(Expression& stmt) {
	return "the default Visitor visit in expr.cpp was called (expression stmt)";
}
std::any Visitor::visitPrintStmt(Print& stmt) {
	return "the default Visitor visit in expr.cpp was called (print stmt)";
}
std::any Visitor::visitVarStmt(Var& stmt) {
	return "the default Visitor visit in expr.cpp was called (var stmt)";
}

// Expr
// Accept a visitor (see the `visitor pattern`)
std::any Expr::accept(Visitor* visitor) {
	std::string s = "the default Expr accept in expr.cpp was called";
	return s;
}

// Binary
Binary::Binary(Expr& left, Token& op, Expr& right) : left(left), op(op), right(right) { }
std::any Binary::accept(Visitor* visitor) {
	return visitor->visitBinaryExpr(*this);
}

// Grouping
Grouping::Grouping(Expr& expression) : expression(expression) { }
std::any Grouping::accept(Visitor* visitor) {
	return visitor->visitGroupingExpr(*this);
}

// Literal
Literal::Literal(LoxObject& obj) : obj(obj) { }
std::any Literal::accept(Visitor* visitor) {
	std::cout << "      DBG: in Literal, accepting a visitor" << std::endl;
	return visitor->visitLiteralExpr(*this);
}

// Unary
Unary::Unary(Token& op, Expr& right) : op(op), right(right) { }
std::any Unary::accept(Visitor* visitor) {
	return visitor->visitUnaryExpr(*this);
}

// Variable
Variable::Variable(Token& name) : name(name) { }
std::any Variable::accept(Visitor* visitor) {
	return visitor->visitVariableExpr(*this);
}

// Assign
Assign::Assign(Token& name, Expr& value) : name(name), value(value) { }
std::any Assign::accept(Visitor* visitor) {
	return visitor->visitAssignExpr(*this);
}
