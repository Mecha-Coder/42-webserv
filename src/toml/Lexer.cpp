#include "Lexer.hpp"
#include "Token.hpp"
#include <ctype.h>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <istream>
#include <list>
#include <string.h>
#include <string>

Lexer::Lexer(std::istream& in) {
	last_token = Token::UNINITIALISED;
	this->in = &in;
	nc = 0;
	pos = 0;
}

bool Lexer::SkipSpace(std::string& line) {
	if (pos >= line.size())
		return false;
	while (line[pos] == ' ' || line[pos] == '\t')
		pos++;
	if (pos >= line.size())
		return false;
	return true;
}

bool Lexer::TokenizeValue(std::string& line, Token& tk) {
	if (!SkipSpace(line))
		return false;
	switch (line[pos]) {
	case '[':
		MakeToken(&tk, "[", Token::OPENBRACKET);
		pos++;
		break;
	case ',':
		MakeToken(&tk, ",", Token::COMMA);
		pos++;
		break;
	case ']':
		MakeToken(&tk, "]", Token::CLOSEBRACKET);
		pos++;
		break;
	case '"':
	case '\'': {
		size_t size = line.find(line[pos], pos + 1);
		if (size == std::string::npos)
			return false;
		MakeToken(&tk, line.substr(pos + 1, size - pos - 1), Token::QUOTED);
		pos = size + 1;
		break;
	}
	default: {
		if (isalnum(line[pos]) || strchr("_-", line[pos])) {
			size_t size = line.find_first_not_of(
				"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_", pos);
			if (size == std::string::npos)
				return false;
			MakeToken(&tk, line.substr(pos, size - pos), Token::VALUE);
			pos = size;
		} else
			return false;
		break;
	}
	}
	return true;
}

bool Lexer::Tokenize(std::string& line, Token& tk) {
	if (!SkipSpace(line))
		return false;
	switch (line[pos]) {
	case '.':
		MakeToken(&tk, ".", Token::DOT);
		pos++;
		break;
	case '[':
		MakeToken(&tk, "[", Token::OPENBRACKET);
		pos++;
		break;
	case ']':
		MakeToken(&tk, "]", Token::CLOSEBRACKET);
		pos++;
		break;
	case '=':
		MakeToken(&tk, "=", Token::ASSIGN);
		pos++;
		break;
	case '\n':
		MakeToken(&tk, "\n", Token::NEWLINE);
		pos++;
	case '"':
	case '\'': {
		size_t size = line.find(line[pos], pos + 1);
		if (size == std::string::npos)
			return false;
		MakeToken(&tk, line.substr(pos + 1, size - pos - 1), Token::QUOTED);
		pos = size + 1;
		break;
	}
	case '#': {
		size_t size = line.find('\n', pos);
		if (size == std::string::npos)
			return false;
		MakeToken(&tk, line.substr(pos + 1, size), Token::COMMENT);
		pos = size + 1;
		break;
	}
	default:
		if (isalnum(line[pos]) || strchr("_-", line[pos])) {
			size_t size = line.find_first_not_of(
				"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_", pos);
			if (size == std::string::npos)
				return false;
			MakeToken(&tk, line.substr(pos, size - pos), Token::KEY);
			pos = size;
		} else
			return false;
		break;
	}
	return true;
}

Token::e_token Lexer::Expect() {
	switch (last_token) {
	case Token::UNINITIALISED:
		return Token::KEY | Token::COMMENT | Token::NEWLINE | Token::OPENBRACKET;
	case Token::KEY:
		return Token::ASSIGN | Token::DOT | Token::CLOSEBRACKET;
	case Token::ASSIGN:
		return Token::QUOTED | Token::KEY | Token::OPENBRACKET;
	case Token::QUOTED:
		return Token::NEWLINE | Token::COMMA;
	case Token::DOT:
		return Token::KEY;
	case Token::OPENBRACKET:
		return Token::KEY | Token::QUOTED | Token::OPENBRACKET;
	case Token::CLOSEBRACKET:
		return Token::NEWLINE | Token::CLOSEBRACKET | Token::COMMENT;
	case Token::COMMENT:
		return Token::NEWLINE;
	case Token::VALUE:
		return Token::NEWLINE | Token::COMMA;
	case Token::COMMA:
		return Token::VALUE | Token::QUOTED;
	case Token::_EOF:
	case Token::NEWLINE:
		abort();
	}
}

bool Lexer::GetNextLine(std::string& line) {
	if (in->eof())
		return false;
	std::getline(*in, line);
	line.push_back('\n');
	nc++;
	pos = 0;
	last_token = Token::UNINITIALISED;
	return !in->fail();
}

Token::e_token Lexer::ExpectValue() {
	switch (last_token) {
	case Token::QUOTED:
		return Token::NEWLINE | Token::COMMA | Token::COMMENT | Token::CLOSEBRACKET;
	case Token::OPENBRACKET:
		return Token::KEY | Token::QUOTED;
	case Token::CLOSEBRACKET:
		return Token::NEWLINE | Token::COMMENT;
	case Token::COMMENT:
		return Token::NEWLINE;
	case Token::VALUE:
		return Token::NEWLINE | Token::COMMA | Token::COMMENT;
	case Token::COMMA:
		return Token::VALUE | Token::QUOTED;
	case Token::ASSIGN:
		return Token::OPENBRACKET | Token::QUOTED | Token::VALUE;
	case Token::_EOF:
		abort();
	default:
		return (Token::e_token)0;
	}
}

TokenListResult Lexer::ParseValue(std::string& line) {
	Token t;
	std::list<Token> list;
	while (TokenizeValue(line, t)) {
		if (!(t.type & ExpectValue()))
			return ParseError("unexpected token `" + t.as_str() + "`", nc);
		last_token = t.type;
		list.push_back(t);
	}
	return TokenListResult(list);
}

TokenListResult Lexer::Parse() {
	std::string line;
	Token t;
	std::list<Token> list;
	while (GetNextLine(line)) {
		while (Tokenize(line, t)) {
			if (!(t.type & Expect())) {
				return ParseError("unexpected token `" + t.as_str() + "`", nc);
			}
			last_token = t.type;
			list.push_back(t);
			if (last_token == Token::ASSIGN) {
				// = [value]
				TokenListResult res = ParseValue(line);
				if (!res.is_ok())
					return res;
				list.splice(list.end(), res.ok());
				break;
			}
		}
		MakeToken(&t, "\n", Token::NEWLINE);
		list.push_back(t);
	}
	return TokenListResult(list);
}

void Lexer::MakeToken(Token* t, std::string value, Token::e_token type) {
	t->value = value;
	t->type = type;
	t->line = nc;
	t->offset = pos;
}
