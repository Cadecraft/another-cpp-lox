#pragma once

// Data structures

#include <string>

// The token type
enum class TokenType {
	// Single-character tokens
	LeftParen,
	RightParen,
	LeftBrace,
	RightBrace,
	Comma,
	Dot,
	Minus,
	Plus,
	Semicolon,
	Slash,
	Star,
	// One or two character tokens
	Bang,
	BangEqual,
	Equal,
	EqualEqual,
	Greater,
	GreaterEqual,
	Less,
	LessEqual,
	// Literals
	Identifier,
	String,
	Number,
	// Keywords
	And,
	Class,
	Else,
	False,
	Fun,
	For,
	If,
	Nil,
	Or,
	Print,
	Return,
	Super,
	This,
	True,
	Var,
	While,
	EndOfFile
};

// The type of a lox object, which can be a string, number, or other value
enum class LoxObjectType {
	String,
	Number,
	Empty
};
