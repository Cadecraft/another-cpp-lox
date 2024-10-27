#pragma once

#include "token.h"
#include "loxobject.h"

class Binary;
class Grouping;
class Literal;
class Unary;

template <typename T>
class Visitor {
	T visitBinaryExpr(Binary& expr);
	T visitGroupingExpr(Grouping& expr);
	T visitLiteralExpr(Literal& expr);
	T visitUnaryExpr(Unary& expr);
};

class Expr {
public:
	// Constructor
	//Expr();

	// Accept a visitor (see the `visitor pattern`)
	// Originally was virtual
	template <typename R>
	R accept(Visitor<R>& visitor);
};

class Binary : public Expr {
public:
	Expr& left;
	Token& op;
	Expr& right;

	Binary(Expr& left, Token& op, Expr& right);

	// TODO: override with virtual (replace the types with a LoxObject?
	// TODO: implement accept for all
	template <typename R>
	R accept(Visitor<R>& visitor);
};

class Grouping : public Expr {
public:
	Expr& expression;

	Grouping(Expr& expression);

	template <typename R>
	R accept(Visitor<R>& visitor);
};

class Literal : public Expr {
public:
	LoxObject& obj;

	Literal(LoxObject& obj);

	template <typename R>
	R accept(Visitor<R>& visitor);
};

class Unary : public Expr {
public:
	Token& op;
	Expr& right;

	Unary(Token& op, Expr& right);

	template <typename R>
	R accept(Visitor<R>& visitor);
};
