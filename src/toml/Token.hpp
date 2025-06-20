#pragma once

#include "Result.hpp"
#include "TomlTypedef.hpp"
#include "ParseError.hpp"
#include <istream>
#include <string>

class Token {
public:
	enum e_token {
		UNINITIALISED	= 1,
		KEY				= 1 << 1,
		ASSIGN			= 1 << 2,
		QUOTE			= 1 << 3,
		DOT				= 1 << 4,
		OPAREN			= 1 << 5,
		CPAREN			= 1 << 6,
		NEWLINE			= 1 << 7,
		COMMA			= 1 << 8,
		COMMENT			= 1 << 9,
		VALUE			= 1 << 10,
		_EOF			= 1 << 20
	};

	e_token type;
	std::string value;
	int line;
	int offset;

	Token();
	bool Is(e_token t) const;
	std::string asStr() const;
};

inline Token::e_token operator|(Token::e_token t1, Token::e_token t2) {
	return (Token::e_token)((int)t1 | (int)t2);
}

class TokenParser {
public:
	TokenParser(std::istream& input);
	TokenListResult Parse();

private:
	std::istream* in;
	std::string line;
	size_t pos;
	size_t nc;
	Token::e_token last_token;

	bool SkipSpace(std::string& line);
	bool GetNextLine(std::string& line);
	void MakeToken(Token* t, const std::string& value, Token::e_token type);
	Token::e_token Expect() const;
	Token::e_token ExpectValue() const;

	bool Tokenize(std::string& line, Token& tk);
	bool TokenizeValue(std::string& line, Token& tk);
	TokenListResult ParseValue(std::string& line);
};
