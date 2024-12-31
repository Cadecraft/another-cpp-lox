#pragma once

#include "expr.h"
#include <string>
#include <any>
#include <iostream>
#include <typeinfo>

class AstPrinter : public Visitor {
public:
	std::string print(Expr& expr) {
		std::string s = std::any_cast<std::string>(expr.accept(this));
		return s;
	}

	std::any visitBinaryExpr(Binary& expr) {
		std::string s = parenthesize(expr.op.lexeme, expr.left, expr.right);
		return s;
	}

	std::any visitGroupingExpr(Grouping& expr) {
		std::string s = "group";
		std::string res = parenthesize(s, expr.expression);
		return res;
	}

	std::any visitLiteralExpr(Literal& expr) {
		if (expr.obj.isEmpty()) {
			std::string s = "nil";
			return s;
		} else {
			return expr.obj.toString();
		}
	}

	std::any visitUnaryExpr(Unary& expr) {
		std::string res = parenthesize(expr.op.lexeme, expr.right);
		return res;
	}

	std::string parenthesize(std::string& name, Expr& expr1) {
		std::string res = "(";
		res += name;
		res += " ";
		res += std::any_cast<std::string>(expr1.accept(this));
		res += ")";
		return res;
	}

	std::string parenthesize(std::string& name, Expr& expr1, Expr& expr2) {
		std::string res = "(";
		res += name;
		res += " ";
		res += std::any_cast<std::string>(expr1.accept(this));
		res += " ";
		res += std::any_cast<std::string>(expr2.accept(this));
		res += ")";
		return res;
	}
};
