#pragma once

#include "expr.h"
#include "runtimeerror.h"
#include "loxobject.h"
#include "structures.h"
#include "environment.h"
#include <string>
#include <any>
#include <iostream>
#include <typeinfo> // For debugging
#include <vector>

class Interpreter : public Visitor {
private:
	Environment environment;

public:
	// Visit a literal (return a LoxObject)
	std::any visitLiteralExpr(Literal& expr);

	// Visit a group (return a LoxObject)
	std::any visitGroupingExpr(Grouping& expr);

	// Evaluate an expression
	std::any evaluate(Expr& expr);

	// Visit a unary expression (return a LoxObject)
	std::any visitUnaryExpr(Unary& expr);

	// Visit a variable expression
	std::any visitVariableExpr(Variable& expr);

	// Visit an assign expression
	std::any visitAssignExpr(Assign& expr);

	// Visit a block statement
	std::any visitBlockStmt(Block& stmt);

	// Visit an expression statement (return null)
	std::any visitExpressionStmt(Expression& stmt);

	// Visit a print statement (return null)
	std::any visitPrintStmt(Print& stmt);

	// Visit a var statement
	std::any visitVarStmt(Var& stmt);

	// Check if an object is truthy or not
	bool isTruthy(LoxObject& obj);

	// Check if an object is equal to another object
	bool isEqual(LoxObject* a, LoxObject* b);

	// Check that the operands are valid, if double
	void checkNumberOperand(Token& oper, LoxObject& operand);

	// Check that the operands are valid, if double
	void checkNumberOperands(Token& oper, LoxObject& left, LoxObject& right);

	// Execute a block
	void executeBlock(std::vector<Stmt*>& statements, Environment newEnviron);

	// Execute a statement
	void execute(Stmt& stmt);

	// Evaluate binary operators
	std::any visitBinaryExpr(Binary& expr);

	// NOTE: this was implemented before the other parts of the interpreter,
	// just so that we can test running it
	void interpret(std::vector<Stmt*>& statements);
};
