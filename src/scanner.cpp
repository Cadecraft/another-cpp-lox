#include "scanner.h"

// Return whether we have consumed all the characters
bool Scanner::isAtEnd() {
	return current >= ((int) source.length());
}

// Return the current then advance ahead
char Scanner::advance() {
	current++;
	return source[current - 1];
}

// Add a token of a specific type
void Scanner::addToken(TokenType type) {
	LoxObject l;
	addToken(type, l);
}

// Add a token
// Literal is allowed to be a string, int, or other object (TODO: refactor)
void Scanner::addToken(TokenType type, LoxObject literal) {
	// TODO: impl
	std::string text = source.substr(start, current - start);
	tokens.push_back(new Token(type, text, literal, line));
}

// Check if match, and if so, consume the current character (like conditional version of `advance()`)
bool Scanner::match(char expected) {
	if (isAtEnd()) return false;
	if (source[current] != expected) return false;
	current++;
	return true;
}

// Advance without consuming the character (technically called "lookahead")
char Scanner::peek() {
	if (isAtEnd()) return '\0';
	return source[current];
}

// Advance twice without consuming the characters
char Scanner::peekNext() {
	if (current + 1 >= (int) (source.length())) return '\0';
	return source[current + 1];
}

// String
void Scanner::string() {
	// TODO: fix string scanning (seems to be an issue)
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
	// Subtract two to remove the quote (since based on length)
	std::string value = source.substr(start + 1, current - start - 2);
	addToken(TokenType::String, value);
}

// Check whether a character is a digit
bool Scanner::isDigit(char c) {
	return c >= '0' && c <= '9';
}

// Check whether a character is an alpha character
bool Scanner::isAlpha(char c) {
	return (
		(c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| (c == '_')
	);
}

// Check whether a number is an alpha character or a digit
bool Scanner::isAlphaNumeric(char c) {
	return isAlpha(c) || isDigit(c);
}

// Number
void Scanner::number() {
	// Consume the literal
	while (isDigit(peek())) advance();
	// Look for a fractional part
	if (peek() == '.' && isDigit(peekNext())) {
		// Consume the `.`
		advance();
		while (isDigit(peek())) {
			advance();
		}
	}
	try {
		// Add the number as a token
		addToken(TokenType::Number, std::stod(source.substr(start, current - start)));
	} catch(std::invalid_argument& e) {
		Lox::error(line, "Invalid number.");
	}
}

// Identifier
void Scanner::identifier() {
	while (isAlphaNumeric(peek())) advance();
	std::string text = source.substr(start, current - start);
	if (keywords.find(text) == keywords.end()) {
		// User-defined identifier
		addToken(TokenType::Identifier);
	} else {
		// Keyword
		addToken(keywords[text]);
	}
}

// Scan an individual token
void Scanner::scanToken() {
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
		// Check for digits or alpha characters
		if (isDigit(c)) {
			number();
			break;
		} else if (isAlpha(c)) {
			identifier();
			break;
		}
		// Character is still consumed and we keep scanning, but errors
		Lox::error(line, "Unexpected character.");
		break;
	}
}

// Create the scanner with a source string
Scanner::Scanner(std::string source) {
	this->source = source;
	this->start = 0;
	this->current = 0;
	this->line = 1;
	// Store keywords in a map (for ease)
	keywords["and"] = TokenType::And;
	keywords["class"] = TokenType::Class;
	keywords["else"] = TokenType::Else;
	keywords["false"] = TokenType::False;
	keywords["for"] = TokenType::For;
	keywords["fun"] = TokenType::Fun;
	keywords["if"] = TokenType::If;
	keywords["nil"] = TokenType::Nil;
	keywords["or"] = TokenType::Or;
	keywords["print"] = TokenType::Print;
	keywords["return"] = TokenType::Return;
	keywords["super"] = TokenType::Super;
	keywords["this"] = TokenType::This;
	keywords["true"] = TokenType::True;
	keywords["var"] = TokenType::Var;
	keywords["while"] = TokenType::While;
}

// Scan the tokens
std::vector<Token*> Scanner::scanTokens() {
	while (!isAtEnd()) {
		// Beginning of next lexeme
		start = current;
		scanToken();
	}
	// Empty token
	LoxObject l;
	tokens.push_back(new Token(TokenType::EndOfFile, "", l, line));
	std::vector<Token*> res(tokens);
	return res;
}
