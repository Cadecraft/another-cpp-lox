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

class If : public Stmt {
public:
	Expr& condition;
	Stmt& thenBranch;
	Stmt* elseBranch; // NOTE: the else branch could be nonexistent (nullptr)

	// TODO: should these actually claim ownership of the items, instead of taking references?
	// TODO: ^ for improving/fixing memory management in the future
	If(Expr& condition, Stmt& thenBranch, Stmt* elseBranch);

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

	Var(Token& name, Expr* initializer);

	std::any accept(Visitor* visitor);
};

class While : public Stmt {
public:
	Expr& condition;
	Stmt& body;

	While(Expr& condition, Stmt& body);

	std::any accept(Visitor* visitor);
};
