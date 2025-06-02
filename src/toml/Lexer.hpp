#pragma once

#include "ParseError.hpp"
#include "Result.hpp"
#include "Token.hpp"
#include <iostream>
#include <list>
#include <stddef.h>
#include <string>

typedef Result<Token*, ParseError> ParseResult;
typedef Result<std::list<Token>, ParseError> TokenListResult;

class Lexer {

public:
	Lexer(std::istream&);
	TokenListResult Parse();

private:
	std::istream* in;
	std::string line;
	size_t pos;
	size_t nc;
	Token::e_token last_token;

	// utils

	bool SkipSpace(std::string& line);

	// cursor
	char next_char();
	char current();
	bool GetNextLine(std::string& line);
	void MakeToken(Token* t, std::string value, Token::e_token type);
	Token::e_token Expect();
	Token::e_token ExpectValue();

	// new
	TokenListResult ParseValue(std::string& line);
	// tokenizer
	bool Tokenize(std::string& line, Token& tk);
	bool TokenizeValue(std::string& line, Token& tk);
	// validation
	//bool expected(Token::e_token type);
};
