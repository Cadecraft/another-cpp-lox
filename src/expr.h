#include "token.h"
#include "loxobject.h"

class Binary;

template <typename T>
class Visitor {
	T visitBinaryExpr(Binary& b) {
		// Do stuff
	}
};

class Expr {
public:
	// Constructor
	//Expr();

	// Accept a visitor (see the `visitor pattern`)
	// Originally was virtual
	template <typename R>
	R accept(Visitor<R>& visitor);
};

class Binary : protected Expr {
public:
	Expr& left;
	Token& op;
	Expr& right;

	Binary(Expr& left, Token& op, Expr& right) : left(left), op(op), right(right) { }

	// TODO: override with virtual (replace the types with a LoxObject?
	// TODO: implement accept for all
	template <typename R>
	R accept(Visitor<R>& visitor) {
		return visitor.visitBinaryExpr(this);
	}
};

class Grouping : protected Expr {
public:
	Expr& expression;

	Grouping(Expr& expression) : expression(expression) { }
};

class Literal : protected Expr {
public:
	LoxObject& obj;

	Literal(LoxObject& obj) : obj(obj) { }
};

class Unary : protected Expr {
public:
	Token& op;
	LoxObject& right;

	Unary(Token& op, LoxObject& right) : op(op), right(right) { }
};
