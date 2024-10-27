#include "expr.h"

class Binary;

// Visitor
// TODO: template necessary?
template <typename T>
T Visitor<T>::visitBinaryExpr(Binary& b) {
	// Do stuff
}

// Expr
// Accept a visitor (see the `visitor pattern`)
// Defined in the header

// Binary
Binary::Binary(Expr& left, Token& op, Expr& right) : left(left), op(op), right(right) { }
// TODO: override with virtual (replace the types with a LoxObject?
// TODO: implement accept for all
template <typename R>
R Binary::accept(Visitor<R>& visitor) {
	return visitor.visitBinaryExpr(*this);
}

// Grouping
Grouping::Grouping(Expr& expression) : expression(expression) { }
template <typename R>
R Grouping::accept(Visitor<R>& visitor) {
	return visitor.visitGroupingExpr(*this);
}

// Literal
Literal::Literal(LoxObject& obj) : obj(obj) { }
template <typename R>
R Literal::accept(Visitor<R>& visitor) {
	return visitor.visitLiteralExpr(*this);
}

// Unary
Unary::Unary(Token& op, Expr& right) : op(op), right(right) { }
template <typename R>
R Unary::accept(Visitor<R>& visitor) {
	return visitor.visitUnaryExpr(*this);
}
