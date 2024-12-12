#pragma once

#include "lox.h" // TODO: cyclical dependency?
#include "expr.h"
#include "runtimeerror.h"
#include "LoxObject.h"
#include "structures.h"
#include <string>
#include <any>
#include <iostream>

class Interpreter : public Visitor {
public:
	// Visit a literal (return a LoxObject)
	std::any visitLiteralExpression(Literal& expr) {
		return expr.obj;
	}

	// Visit a group (return a LoxObject)
	std::any visitGrouping(Grouping& expr) {
		return evaluate(expr.expression);
	}

	// Evaluate an expression
	std::any evaluate(Expr& expr) {
		std::cout << "    DBG: in evaluate, about to expr.accept" << std::endl;
		return expr.accept(this);
	}

	// Visit a unary expression (return a LoxObject)
	std::any visitUnaryExpr(Unary& expr) {
		LoxObject* right = std::any_cast<LoxObject*>(evaluate(expr.right));
		switch (expr.op.type) {
		case TokenType::Bang: {
			LoxObject res((isTruthy(*right) ? 0.0 : 1.0));
			return res;
		}
		case TokenType::Minus: {
			checkNumberOperand(expr.op, *right);
			LoxObject res(-1 * ((double) right->getNumberValue()));
			return res;
		}
		default: {
			// Any/"unreachable"
			LoxObject res;
			return res;
		}
		}
	}

	// Check if an object is truthy or not
	bool isTruthy(LoxObject& obj) {
		if (obj.isEmpty()) {
			return false;
		}
		if (obj.getType() == LoxObjectType::Number) {
			// Only falsy if the value of the number is 0
			return obj.getNumberValue() != 0;
		}
		// Assume truthy otherwise
		return true;
	}

	// Check if an object is equal to another object
	bool isEqual(LoxObject* a, LoxObject* b) {
		if (a->isEmpty() && b->isEmpty()) return true;
		if (a->isEmpty()) return false;
		if (a->getType() == LoxObjectType::Number && b->getType() == LoxObjectType::Number) {
			return a->getNumberValue() == b->getNumberValue();
		}
		if (a->getType() == LoxObjectType::String && b->getType() == LoxObjectType::String) {
			return a->getStringVal() == b->getStringVal();
		}
		// Should never reach this point; this means the types are mismatched
		return false;
	}

	// Check that the operands are valid, if double
	// TODO: impl
	void checkNumberOperand(Token& oper, LoxObject& operand) {
		if (operand.getType() == LoxObjectType::Number) return;
		// Invalid operand
		throw RuntimeError(oper, "operand must be a number");
	}

	// Check that the operands are valid, if double
	// TODO: impl
	void checkNumberOperands(Token& oper, LoxObject& left, LoxObject& right) {
		if (left.getType() == LoxObjectType::Number && right.getType() == LoxObjectType::Number) return;
		// Invalid operand
		throw RuntimeError(oper, "operands must be numbers");
	}

	// Evaluate binary operators
	std::any visitBinaryExpr(Binary& expr) {
		LoxObject* left = std::any_cast<LoxObject*>(evaluate(expr.left));
		LoxObject* right = std::any_cast<LoxObject*>(evaluate(expr.right));
		switch (expr.op.type) {
		case TokenType::Slash: {
			checkNumberOperands(expr.op, *right, *left);
			LoxObject res(left->getNumberValue() / right->getNumberValue());
			return res;
		}
		case TokenType::Star: {
			checkNumberOperands(expr.op, *right, *left);
			LoxObject res(left->getNumberValue() * right->getNumberValue());
			return res;
		}
		case TokenType::Minus: {
			checkNumberOperands(expr.op, *right, *left);
			LoxObject res(left->getNumberValue() - right->getNumberValue());
			return res;
		}
		case TokenType::Plus: {
			checkNumberOperands(expr.op, *right, *left);
			if (left->getType() == LoxObjectType::Number && right->getType() == LoxObjectType::Number) {
				LoxObject res(left->getNumberValue() * right->getNumberValue());
				return res;
			}
			if (left->getType() == LoxObjectType::String && right->getType() == LoxObjectType::String) {
				LoxObject res(left->getStringVal() + right->getStringVal());
				return res;
			}
			// Failed to find a valid type
			throw RuntimeError(expr.op, "operands must be two numbers or two strings");
		}
		case TokenType::Greater: {
			checkNumberOperands(expr.op, *right, *left);
			LoxObject res((left->getNumberValue() > right->getNumberValue()) ? 1.0 : 0.0);
			return res;
		}
		case TokenType::GreaterEqual: {
			checkNumberOperands(expr.op, *right, *left);
			LoxObject res((left->getNumberValue() >= right->getNumberValue()) ? 1.0 : 0.0);
			return res;
		}
		case TokenType::Less: {
			checkNumberOperands(expr.op, *right, *left);
			LoxObject res((left->getNumberValue() < right->getNumberValue()) ? 1.0 : 0.0);
			return res;
		}
		case TokenType::LessEqual: {
			checkNumberOperands(expr.op, *right, *left);
			LoxObject res((left->getNumberValue() <= right->getNumberValue()) ? 1.0 : 0.0);
			return res;
		}
		case TokenType::BangEqual: {
			LoxObject res((!isEqual(left, right)) ? 1.0 : 0.0);
			return res;
		}
		case TokenType::EqualEqual: {
			LoxObject res((isEqual(left, right)) ? 1.0 : 0.0);
			return res;
		}
		default: {
			// Unreachable
			LoxObject res;
			return res;
		}
		}
	}

	// TODO: continue implementing
	// NOTE: this was implemented before the other parts of the interpreter,
	// just so that we can test running it
	void interpret(Expr& expr) {
		// TODO: impl try catch
		try {
			std::cout << "  DBG: Evaluating..." << std::endl;
			// TODO: figure out bad_any_cast
			std::any value = evaluate(expr);
			//evaluate(expr);
			// TODO: actually output the value
			std::cout << "  DBG: Evaluation finished" << std::endl;
		} catch (RuntimeError& r) {
			// TODO: correct?
			Lox::error(&r.token, r.message);
			// Lox::runtimeError(error);
		}
	}
};
