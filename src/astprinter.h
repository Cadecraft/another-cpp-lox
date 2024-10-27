#include "expr.h"

class AstPrinter : protected Visitor<std::string> {
public:
	std::string print(Expr& expr) {
		return expr.accept(*this);
	}
	std::string visitBinaryExpr(Binary& expr) {
		return parenthesize(expr.op.lexeme, expr.left, expr.right)
	}
	std::string visitGroupingExpr(Grouping& expr) {
		std::string s = "group";
		return parenthesize(s, expr.expression);
	}
	std::string visitLiteralExpr(Literal& expr) {
		if (expr.obj.isEmpty()) {
			return "nil";
		} else {
			return expr.obj.toString();
		}
	}
	std::string visitUnaryExpr(Unary expr) {
		return parenthesize(expr.op.lexeme, expr.right);
	}
	std::string parenthesize(std::string& name, Expr& expr1) {
		std::string res = "(";
		res += name;
		res += " ";
		res += expr1.accept(*this);
	}
	std::string parenthesize(std::string& name, Expr& expr1, Expr& expr2) {
		std::string res = "(";
		res += name;
		res += " ";
		res += expr1.accept(*this);
		res += " ";
		res += expr2.accept(*this);
		res += ")";
		return res;
	}
};
