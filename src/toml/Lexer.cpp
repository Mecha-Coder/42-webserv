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
		MakeToken(&tk, "[", Token::OPAREN);
		pos++;
		break;
	case ',':
		MakeToken(&tk, ",", Token::COMMA);
		pos++;
		break;
	case ']':
		MakeToken(&tk, "]", Token::CPAREN);
		pos++;
		break;
	case '"':
	case '\'': {
		size_t size = line.find(line[pos], pos + 1);
		if (size == std::string::npos)
			return false;
		MakeToken(&tk, line.substr(pos + 1, size - pos - 1), Token::QUOTE);
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
		MakeToken(&tk, "[", Token::OPAREN);
		pos++;
		break;
	case ']':
		MakeToken(&tk, "]", Token::CPAREN);
		pos++;
		break;
	case '=':
		MakeToken(&tk, "=", Token::ASSIGN);
		pos++;
		break;
	case '\n':
		MakeToken(&tk, "\n", Token::NEWLINE);
		pos++;
		break;
	case '"':
	case '\'': {
		size_t size = line.find(line[pos], pos + 1);
		if (size == std::string::npos)
			return false;
		MakeToken(&tk, line.substr(pos + 1, size - pos - 1), Token::QUOTE);
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
		return Token::KEY | Token::COMMENT | Token::NEWLINE | Token::OPAREN;
	case Token::KEY:
		return Token::ASSIGN | Token::DOT | Token::CPAREN;
	case Token::ASSIGN:
		return Token::QUOTE | Token::KEY | Token::OPAREN;
	case Token::QUOTE:
		return Token::NEWLINE | Token::COMMA;
	case Token::DOT:
		return Token::KEY;
	case Token::OPAREN:
		return Token::KEY | Token::QUOTE | Token::OPAREN;
	case Token::CPAREN:
		return Token::NEWLINE | Token::CPAREN | Token::COMMENT;
	case Token::COMMENT:
		return Token::NEWLINE;
	case Token::VALUE:
		return Token::NEWLINE | Token::COMMA;
	case Token::COMMA:
		return Token::VALUE | Token::QUOTE;
	case Token::_EOF:
	case Token::NEWLINE:
		abort();
	}
	return Token::NEWLINE;
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
	case Token::QUOTE:
		return Token::NEWLINE | Token::COMMA | Token::COMMENT | Token::CPAREN;
	case Token::OPAREN:
		return Token::KEY | Token::QUOTE;
	case Token::CPAREN:
		return Token::NEWLINE | Token::COMMENT;
	case Token::COMMENT:
		return Token::NEWLINE;
	case Token::VALUE:
		return Token::NEWLINE | Token::COMMA | Token::COMMENT;
	case Token::COMMA:
		return Token::VALUE | Token::QUOTE;
	case Token::ASSIGN:
		return Token::OPAREN | Token::QUOTE | Token::VALUE;
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
			return ParseError("unexpected token `" + t.asStr() + "`", nc);
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
				return ParseError("unexpected token `" + t.asStr() + "`", nc);
			}
			last_token = t.type;
			list.push_back(t);
			if (last_token == Token::ASSIGN) {
				// = [value]
				TokenListResult res = ParseValue(line);
				if (!res.isOk())
					return res;
				list.splice(list.end(), res.Ok());
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
