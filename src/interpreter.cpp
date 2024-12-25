#include "interpreter.h"
#include "stmt.h"

#include "lox.h"

std::any Interpreter::visitLiteralExpr(Literal& expr) {
	std::cout << "    DBG: in Interpreter::visitLiteralExpr, returning" << std::endl;
	return expr.obj;
}

std::any Interpreter::visitGroupingExpr(Grouping& expr) {
	return evaluate(expr.expression);
}

std::any Interpreter::evaluate(Expr& expr) {
	std::cout << "    DBG: in evaluate, about to expr.accept (type: " << typeid(expr).name() << ")" << std::endl;
	std::any res = expr.accept(this);
	std::cout << "    DBG: in evaluate, about to return (type: " << typeid(res).name() << ")" << std::endl;
	return res;
}

std::any Interpreter::visitUnaryExpr(Unary& expr) {
	std::any eval_right = evaluate(expr.right);
	LoxObject* right = &std::any_cast<LoxObject&>(eval_right);
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

std::any Interpreter::visitVariableExpr(Variable& expr) {
	return environment.get(expr.name);
}

bool Interpreter::isTruthy(LoxObject& obj) {
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

bool Interpreter::isEqual(LoxObject* a, LoxObject* b) {
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

void Interpreter::checkNumberOperand(Token& oper, LoxObject& operand) {
	if (operand.getType() == LoxObjectType::Number) return;
	// Invalid operand
	throw RuntimeError(oper, "operand must be a number");
}

void Interpreter::checkNumberOperands(Token& oper, LoxObject& left, LoxObject& right) {
	if (left.getType() == LoxObjectType::Number && right.getType() == LoxObjectType::Number) return;
	// Invalid operand
	throw RuntimeError(oper, "operands must be numbers");
}

std::any Interpreter::visitBinaryExpr(Binary& expr) {
	std::cout << "    DBG: inside Interpreter::visitBinaryExpr" << std::endl;
	std::any eval_left = evaluate(expr.left);
	std::any eval_right = evaluate(expr.right);
	LoxObject* dbg_e = new LoxObject();
	std::any dbg_type_tester = dbg_e;
	std::any dbg_type_tester_2 = *dbg_e;
	std::cout << "    DBG: finished evaluating, about to any cast" << std::endl;
	std::cout << "    DBG: Type of eval_left: " << eval_left.type().name() << std::endl;
	std::cout << "    DBG: Type of dbg_type_tester: " << dbg_type_tester.type().name() << std::endl;
	std::cout << "    DBG: Type of dbg_type_tester_2: " << dbg_type_tester_2.type().name() << std::endl;
	std::cout << "    DBG: Type of eval_right: " << eval_right.type().name() << std::endl;
	LoxObject* left = &std::any_cast<LoxObject&>(eval_left);
	LoxObject* right = &std::any_cast<LoxObject&>(eval_right);
	std::cout << "    DBG: finished any casts in Interpreter::visitBinaryExpr" << std::endl;
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
		if (left->getType() == LoxObjectType::Number && right->getType() == LoxObjectType::Number) {
			LoxObject res(left->getNumberValue() + right->getNumberValue());
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

std::any Interpreter::visitExpressionStmt(Expression& stmt) {
	evaluate(stmt.expr);
	// TODO: should return nullptr?
	return nullptr;
}

std::any Interpreter::visitPrintStmt(Print& stmt) {
	std::cout << "DBG: in visit print statement:" << std::endl;
	std::any value = evaluate(stmt.expr);
	// TODO: is it a LoxObject?
	// TODO: refactor into stringify, which was developed in the Evaluating Expressions chapter?
	std::cout << "DBG: PRINT STATEMENT VISITED:" << std::endl;
	std::cout << std::any_cast<LoxObject>(value).toString() << std::endl;
	// TODO: should return nullptr?
	return nullptr;
}

std::any Interpreter::visitVarStmt(Var& stmt) {
	// TODO: check initializer in Var is implemented properly
	if (stmt.initializer != nullptr) {
		// TODO: evaluate should return a LoxObject, right?
		std::any value = evaluate(*stmt.initializer);
		LoxObject value_obj = std::any_cast<LoxObject>(value);
		environment.define(stmt.name.lexeme, value_obj);
		return nullptr;
	} else {
		LoxObject value;
		environment.define(stmt.name.lexeme, value);
		// Empty
		return nullptr;
	}
}

void Interpreter::execute(Stmt& stmt) {
	stmt.accept(this);
}

void Interpreter::interpret(std::vector<Stmt*>& statements) {
	try {
		for (Stmt* stmt : statements) {
			execute(*stmt);
		}
	} catch (RuntimeError& r) {
		Lox::error(&r.token, r.message);
	}
}
