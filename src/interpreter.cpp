#include "interpreter.h"
#include "stmt.h"
#include "debugprinter.h"

#include "lox.h"

std::any Interpreter::visitLiteralExpr(Literal& expr) {
	DebugPrinter::print("in Interpreter::visitLiteralExpr, returning");
	return expr.obj;
}

std::any Interpreter::visitGroupingExpr(Grouping& expr) {
	return evaluate(expr.expression);
}

std::any Interpreter::evaluate(Expr& expr) {
	DebugPrinter::print("in evaluate, about to expr.accept; type: " + DebugPrinter::to_string(typeid(expr).name()));
	std::any res = expr.accept(this);
	DebugPrinter::print("in evaluate, about to return; type: " + DebugPrinter::to_string(typeid(res).name()));
	return res;
}

std::any Interpreter::visitLogicalExpr(Logical& expr) {
	std::any eval_left = evaluate(expr.left);
	LoxObject left = std::any_cast<LoxObject&>(eval_left);
	// Short circuit: only evaluate the right if necessary
	if (expr.op.type == TokenType::Or) {
		if (isTruthy(left)) return left;
	} else {
		if (!isTruthy(left)) return left;
	}
	std::any eval_right = evaluate(expr.right);
	LoxObject right = std::any_cast<LoxObject&>(eval_right);
	return right;
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

std::any Interpreter::visitAssignExpr(Assign& expr) {
	std::any eval_value = evaluate(expr.value);
	LoxObject value = std::any_cast<LoxObject&>(eval_value); 
	environment.assign(expr.name, value);
	// No return needed
	return nullptr;
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
	DebugPrinter::print("inside Interpreter::visitBinaryExpr");
	std::any eval_left = evaluate(expr.left);
	std::any eval_right = evaluate(expr.right);
	DebugPrinter::print("finished evaluating, about to any cast");
	DebugPrinter::print("type of eval_left: " + DebugPrinter::to_string(eval_left.type().name()), 3);
	DebugPrinter::print("type of eval_right: " + DebugPrinter::to_string(eval_right.type().name()), 3);
	LoxObject* left = &std::any_cast<LoxObject&>(eval_left);
	LoxObject* right = &std::any_cast<LoxObject&>(eval_right);
	DebugPrinter::print("finished any casts in Interpreter::visitBinaryExpr");
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

void Interpreter::executeBlock(std::vector<Stmt*>& statements, Environment newEnviron) {
	Environment previous = environment;
	try {
		environment = newEnviron;
		for (Stmt* statement : statements) {
			execute(*statement);
		}
		// Always clean up the resources
		environment = previous;
	} catch(std::runtime_error& e) {
		// The environment will now be re-updated back to the previous one
		// Always clean up the resources (NOTE: in the book, this was done through a `finally` block)
		environment = previous;
		throw e;
	}
}

std::any Interpreter::visitBlockStmt(Block& stmt) {
	Environment newEnviron(environment);
	executeBlock(stmt.statements, newEnviron);
	// TODO: should return nullptr?
	return nullptr;
}

std::any Interpreter::visitExpressionStmt(Expression& stmt) {
	evaluate(stmt.expr);
	// TODO: should return nullptr?
	return nullptr;
}

std::any Interpreter::visitIfStmt(If& stmt) {
	std::any condition_value = evaluate(stmt.condition);
	LoxObject condition_value_obj = std::any_cast<LoxObject>(condition_value);
	if (isTruthy(condition_value_obj)) {
		execute(stmt.thenBranch);
	} else if (stmt.elseBranch != nullptr) { // TODO: FIX by making a POINTER to the else branch
		execute(*(stmt.elseBranch));
	}
	// TODO: should return nullptr?
	return nullptr;
}

std::any Interpreter::visitPrintStmt(Print& stmt) {
	DebugPrinter::print("in visit print statement:");
	std::any value = evaluate(stmt.expr);
	LoxObject castedValue = std::any_cast<LoxObject>(value);
	// TODO: is it a LoxObject?
	// TODO: refactor into stringify, which was developed in the Evaluating Expressions chapter?
	DebugPrinter::print("PRINT statement visited:" + castedValue.toString());
	std::cout << castedValue.toString() << std::endl;
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
