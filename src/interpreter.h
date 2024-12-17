#pragma once

#include "expr.h"
#include "runtimeerror.h"
#include "loxobject.h"
#include "structures.h"
#include <string>
#include <any>
#include <iostream>
#include <typeinfo> // For debugging

class Interpreter : public Visitor {
public:
	// Visit a literal (return a LoxObject)
	std::any visitLiteralExpr(Literal& expr);

	// Visit a group (return a LoxObject)
	std::any visitGroupingExpr(Grouping& expr);

	// Evaluate an expression
	std::any evaluate(Expr& expr);

	// Visit a unary expression (return a LoxObject)
	std::any visitUnaryExpr(Unary& expr);

	// Check if an object is truthy or not
	bool isTruthy(LoxObject& obj);

	// Check if an object is equal to another object
	bool isEqual(LoxObject* a, LoxObject* b);

	// Check that the operands are valid, if double
	void checkNumberOperand(Token& oper, LoxObject& operand);

	// Check that the operands are valid, if double
	void checkNumberOperands(Token& oper, LoxObject& left, LoxObject& right);

	// Evaluate binary operators
	std::any visitBinaryExpr(Binary& expr);

	// NOTE: this was implemented before the other parts of the interpreter,
	// just so that we can test running it
	void interpret(Expr& expr);
};
