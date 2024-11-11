#pragma once

#include "expr.h"
#include "LoxObject.h"
#include "structures.h"
#include <string>
#include <any>

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
		return expr.accept(this);
	}

	// Visit a unary expression (return a LoxObject)
	std::any visitUnaryExpr(Unary& expr) {
		LoxObject* right = evaluate(expr.right);
		switch (expr.operator.type) {
		case TokenType::Bang:
			LoxObject res((isTruthy(right) ? 0.0, 1.0));
			return res;
		case TokenType::Minus:
			LoxObject res(-1 * ((double) right));
			return res;
		default:
			// Any/"unreachable"
			LoxObject res;
			return res;
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
	bool isEqual(LoxObject& a, LoxObject& b) {
		if (a.isEmpty() && b.isEmpty()) return true;
		if (a.isEmpty()) return false;
		if (a.getType() == LoxObjectType::Number && b.getType() == LoxObjectType::Number) {
			return a.getNumberValue() == b.getNumberValue();
		}
		if (a.getType() == LoxObjectType::String && b.getType() == LoxObjectType::String) {
			return a.getStringVal() == b.getStringVal();
		}
		// Should never reach this point; this means the types are mismatched
		return false;
	}

	// Evaluate binary operators
	std::any visitBinaryExpr(Binary& expr) {
		LoxObject& left = evaluate(expr.left);
		LoxObject& right = evaluate(expr.right);
		switch (expr.operator.type) {
		case TokenType::Slash:
			LoxObject res(left.getNumberValue() / right.getNumberValue());
			return res;
		case TokenType::Star:
			LoxObject res(left.getNumberValue() * right.getNumberValue());
			return res;
		case TokenType::Minus:
			LoxObject res(left.getNumberValue() - right.getNumberValue());
			return res;
		case TokenType::Plus:
			if (left.getType() == LoxObjectType::Number && right.getType() == LoxObjectType::Number) {
				LoxObject res(left.getNumberValue() * right.getNumberValue());
				return res;
			}
			if (left.getType() == LoxObjectType::String && right.getType() == LoxObjectType::String) {
				LoxObject res(left.getStringVal() * right.getStringVal());
				return res;
			}
			break;
		case TokenType::Greater:
			LoxObject res((left.getNumberValue() > right.getNumberValue()) ? 1.0 : 0.0);
			return res;
		case TokenType::Greater_Equal:
			LoxObject res((left.getNumberValue() >= right.getNumberValue()) ? 1.0 : 0.0);
			return res;
		case TokenType::Less:
			LoxObject res((left.getNumberValue() < right.getNumberValue()) ? 1.0 : 0.0);
			return res;
		case TokenType::Less_Equal:
			LoxObject res((left.getNumberValue() <= right.getNumberValue()) ? 1.0 : 0.0);
			return res;
		case TokenType::BangEqual:
			LoxObject res((!isEqual(left, right)) ? 1.0 : 0.0);
			return res;
		case TokenType::EqualEqual:
			LoxObject res((isEqual(left, right)) ? 1.0 : 0.0);
			return res;
		default:
			// Unreachable
			LoxObject res;
			return res;
		}
	}

	//
}
