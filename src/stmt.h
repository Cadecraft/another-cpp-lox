#pragma once

#include "expr.h"
#include <any>

class Visitor;
class Expr;

class Stmt {
public:
	// Accept a visitor (see the `visitor pattern`)
	virtual std::any accept(Visitor* visitor);
};

class Expression : public Stmt {
public:
	Expr& expr;

	Expression(Expr& expr);

	std::any accept(Visitor* visitor);
};

class Print : public Stmt {
public:
	Expr& expr;

	Print(Expr& expr);

	std::any accept(Visitor* visitor);
};
