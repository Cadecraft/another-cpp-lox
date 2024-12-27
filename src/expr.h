#pragma once

#include "token.h"
#include "loxobject.h"
#include "stmt.h"
#include <any>

class Binary;
class Grouping;
class Literal;
class Unary;
class Variable;
class Assign;

class Block; // A statement containing a list of statements
class Expression;
class Print;
class Var;

class Visitor {
public:
	virtual std::any visitBinaryExpr(Binary& expr);
	virtual std::any visitGroupingExpr(Grouping& expr);
	virtual std::any visitLiteralExpr(Literal& expr);
	virtual std::any visitUnaryExpr(Unary& expr);
	virtual std::any visitVariableExpr(Variable& expr);
	virtual std::any visitAssignExpr(Assign& expr);
	virtual std::any visitBlockStmt(Block& stmt);
	virtual std::any visitExpressionStmt(Expression& stmt);
	virtual std::any visitPrintStmt(Print& stmt);
	virtual std::any visitVarStmt(Var& stmt);
};

class Expr {
public:
	// Accept a visitor (see the `visitor pattern`)
	virtual std::any accept(Visitor* visitor);
	// TODO: should Expr have a virtual destructor?
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

class Variable : public Expr {
public:
	Token& name;

	Variable(Token& name);

	std::any accept(Visitor* visitor);
};

class Assign : public Expr {
public:
	Token& name;
	Expr& value;

	Assign(Token& name, Expr& value);

	std::any accept(Visitor* visitor);
};
