#include <string>
#include <vector>

#include "structures.h"
#include "token.h"

class Scanner {
private:
	std::string source;
	std::vector<Token*> tokens;

	int start;
	int current;
	int line;
	
	// Return whether we have consumed all the characters
	bool isAtEnd() {
		return current >= ((int) source.length());
	}

	// Return the current then advance ahead
	char advance() {
		current++;
		return source[current - 1];
	}

	// Add a token of a specific type
	void addToken(TokenType type) {
		LoxObject l;
		addToken(type, l);
	}

	// Add a token
	// Literal is allowed to be a string, int, or other object (TODO: refactor)
	void addToken(TokenType type, LoxObject literal) {
		// TODO: impl
		std::string text = source.substr(start, current - start + 1);
		tokens.push_back(new Token(type, text, literal, line));
	}
	
	// Check if match, and if so, consume the current character (like conditional version of `advance()`)
	bool match(char expected) {
		if (isAtEnd()) return false;
		if (source[current] != expected) return false;
		current++;
		return true;
	}

	// Advance without consuming the character (technically called "lookahead")
	char peek() {
		if (isAtEnd()) return '\0';
		return source[current];
	}
	
	void string() {
		while (peek() != '"' && !isAtEnd()) {
			if (peek() == '\n') line++;
			advance();
		}
		if (isAtEnd()) {
			Lox::error(line, "Unterminated string.");
			return;
		}
		// The closing "
		advance();
		// Trim surrounding quotes
		std::string value = source.substr(start + 1, current - start);
		addToken(TokenType::String, value);
	}

	// Scan an individual token
	void scanToken() {
		char c = advance();
		switch (c) {
		// Single-character tokens
		case '(':
			addToken(TokenType::LeftParen);
			break;
		case ')':
			addToken(TokenType::RightParen);
			break;
		case '{':
			addToken(TokenType::LeftBrace);
			break;
		case '}':
			addToken(TokenType::RightBrace);
			break;
		case ',':
			addToken(TokenType::Comma);
			break;
		case '.':
			addToken(TokenType::Dot);
			break;
		case '-':
			addToken(TokenType::Minus);
			break;
		case '+':
			addToken(TokenType::Plus);
			break;
		case ';':
			addToken(TokenType::Semicolon);
			break;
		case '*':
			addToken(TokenType::Star);
			break; 
		// Two-character tokens
		case '!':
			addToken(match('=') ? TokenType::BangEqual : TokenType::Bang);
			break;
		case '=':
			addToken(match('=') ? TokenType::EqualEqual : TokenType::Equal);
			break;
		case '<':
			addToken(match('=') ? TokenType::LessEqual : TokenType::Less);
			break;
		case '>':
			addToken(match('=') ? TokenType::GreaterEqual : TokenType::Greater);
			break;
		case '/':
			if (match('/')) {
				// A comment goes until the end of the line
				while (peek() != '\n' && !isAtEnd()) advance();
			} else {
				addToken(TokenType::Slash);
			}
			break;
		case ' ':
		case '\r':
		case '\t':
			// Ignore whitespace
			break;
		case '\n':
			line++;
			break;
		case '"':
			string();
			break;
		default:
			// Character is still consumed and we keep scanning, but errors
			Lox::error(line, "Unexpected character.");
			break;
		}
	}

public:
	// Create the scanner with a source string
	Scanner(std::string source) {
		this->source = source;
		this->start = 0;
		this->current = 0;
		this->line = 1;
	}

	// Scan the tokens
	void scanTokens() {
		while (!isAtEnd()) {
			// Beginning of next lexeme
			start = current;
			scanToken();
		}
		// Empty token
		LoxObject l;
		tokens.push_back(new Token(TokenType::EndOfFile, "", l, line));
	}
};
