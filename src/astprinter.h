#pragma once

#include "expr.h"
#include <string>
#include <any>

class AstPrinter : public Visitor {
public:
	std::string print(Expr& expr) {
		//return expr.dbg_accept(this);
		std::cout << "dbg: about to print" << std::endl;
		return std::any_cast<std::string>(expr.accept(this));
	}
	std::any visitBinaryExpr(Binary& expr) {
		std::cout << "visit binary expr starting" << std::endl;
		std::string s = parenthesize(expr.op.lexeme, expr.left, expr.right);
		std::cout << "visit binary expr returned: " << s << std::endl;
		return s;
	}
	std::any visitGroupingExpr(Grouping& expr) {
		std::string s = "group";
		return parenthesize(s, expr.expression);
	}
	std::any visitLiteralExpr(Literal& expr) {
		if (expr.obj.isEmpty()) {
			std::string s = "nil";
			return s;
		} else {
			return expr.obj.toString();
		}
	}
	std::any visitUnaryExpr(Unary expr) {
		return parenthesize(expr.op.lexeme, expr.right);
	}
	std::string parenthesize(std::string& name, Expr& expr1) {
		std::string res = "(";
		res += name;
		res += " ";
		res += std::any_cast<std::string>(expr1.accept(this));
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
