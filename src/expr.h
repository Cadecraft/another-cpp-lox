#pragma once

#include "token.h"
#include "loxobject.h"
#include "stmt.h"
#include <any>

class Binary;
class Grouping;
class Literal;
class Unary;

class Expression;
class Print;

class Visitor {
public:
	virtual std::any visitBinaryExpr(Binary& expr);
	virtual std::any visitGroupingExpr(Grouping& expr);
	virtual std::any visitLiteralExpr(Literal& expr);
	virtual std::any visitUnaryExpr(Unary& expr);
	virtual std::any visitExpressionStmt(Expression& stmt);
	virtual std::any visitPrintStmt(Print& stmt);
};

class Expr {
public:
	// Accept a visitor (see the `visitor pattern`)
	virtual std::any accept(Visitor* visitor);
};

class Binary : public Expr {
public:
	Expr& left;
	Token& op;
	Expr& right;

	Binary(Expr& left, Token& op, Expr& right);

	std::any accept(Visitor* visitor);
};

class Grouping : public Expr {
public:
	Expr& expression;

	Grouping(Expr& expression);

	std::any accept(Visitor* visitor);
};

class Literal : public Expr {
public:
	LoxObject& obj;

	Literal(LoxObject& obj);

	std::any accept(Visitor* visitor);
};

class Unary : public Expr {
public:
	Token& op;
	Expr& right;

	Unary(Token& op, Expr& right);

	std::any accept(Visitor* visitor);
};
