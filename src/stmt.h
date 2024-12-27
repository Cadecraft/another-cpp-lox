#pragma once

#include "expr.h"
#include "token.h"
#include <any>
#include <vector>

class Visitor;
class Expr;

class Stmt {
public:
	// Accept a visitor (see the `visitor pattern`)
	virtual std::any accept(Visitor* visitor);
};

class Block : public Stmt {
public:
	std::vector<Stmt*> statements;

	Block(std::vector<Stmt*> statements);

	std::any accept(Visitor* visitor);
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

class Var : public Stmt {
public:
	Token& name;
	Expr* initializer;

	// TODO: is this valid syntax?
	Var(Token& name, Expr* initializer);

	std::any accept(Visitor* visitor);
};
