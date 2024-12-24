#pragma once

#include "expr.h"
#include <string>
#include <any>
#include <iostream>
#include <typeinfo>

class AstPrinter : public Visitor {
public:
	std::string print(Expr& expr) {
		std::cout << "    DBG: inside AstPrinter::print" << std::endl;
		std::string s = std::any_cast<std::string>(expr.accept(this));
		std::cout << "    DBG: finished AstPrinter::print" << std::endl;
		return s;
	}

	std::any visitBinaryExpr(Binary& expr) {
		std::cout << "    DBG: inside AstPrinter::visitBinaryExpr" << std::endl;
		std::string s = parenthesize(expr.op.lexeme, expr.left, expr.right);
		std::cout << "    DBG: finished AstPrinter::visitBinaryExpr" << std::endl;
		return s;
	}

	std::any visitGroupingExpr(Grouping& expr) {
		std::cout << "    DBG: inside AstPrinter::visitGroupingExpr" << std::endl;
		std::string s = "group";
		std::string res = parenthesize(s, expr.expression);
		std::cout << "    DBG: finished AstPrinter::visitGroupingExpr" << std::endl;
		return res;
	}

	std::any visitLiteralExpr(Literal& expr) {
		std::cout << "    DBG: inside AstPrinter::visitLiteralExpr" << std::endl;
		if (expr.obj.isEmpty()) {
			std::string s = "nil";
			std::cout << "    DBG: finished AstPrinter::visitLiteralExpr (empty object)" << std::endl;
			return s;
		} else {
			std::cout << "    DBG: finished AstPrinter::visitLiteralExpr (non-empty object)" << std::endl;
			return expr.obj.toString();
		}
	}

	std::any visitUnaryExpr(Unary& expr) {
		std::cout << "    DBG: inside AstPrinter::visitUnaryExpr" << std::endl;
		std::cout << "    DBG: expr is of type: " << typeid(expr).name() << std::endl;
		std::cout << "    DBG: expr right is of type: " << typeid(expr.right).name() << std::endl;
		std::string res = parenthesize(expr.op.lexeme, expr.right);
		std::cout << "    DBG: finished AstPrinter::visitUnaryExpr" << std::endl;
		return res;
	}

	std::string parenthesize(std::string& name, Expr& expr1) {
		std::string res = "(";
		res += name;
		res += " ";
		std::cout << "      DBG: inside parenthesize (name, expr1), about to any cast" << std::endl;
		std::cout << "      DBG: expr1 (type: " << typeid(expr1).name() << ")" << std::endl;
		std::cout << "      DBG: about to any cast" << std::endl;
		res += std::any_cast<std::string>(expr1.accept(this));
		res += ")";
		std::cout << "      DBG: finished parenthesize (name, expr1)" << std::endl;
		return res;
	}

	std::string parenthesize(std::string& name, Expr& expr1, Expr& expr2) {
		std::string res = "(";
		res += name;
		res += " ";
		std::cout << "      DBG: inside parenthesize (name, expr1, expr2), about to any cast expr1" << std::endl;
		res += std::any_cast<std::string>(expr1.accept(this));
		res += " ";
		std::cout << "      DBG: inside parenthesize (name, expr1, expr2), about to any cast expr2" << std::endl;
		res += std::any_cast<std::string>(expr2.accept(this));
		std::cout << "      DBG: finished parenthesize (name, expr1, expr2)" << std::endl;
		res += ")";
		return res;
	}
};
