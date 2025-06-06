#include "Token.hpp"
#include <cctype>
#include <cstdlib>
#include <string.h>

Token::Token() : type(UNINITIALISED), line(0), offset(0) {}

bool Token::Is(e_token t) const {
	return (type & t) != 0;
}

std::string Token::asStr() const {
	char buf[128];
	snprintf(buf, sizeof(buf), "TOKEN %d:%d `%s`", line, offset, value.c_str());
	std::string s = buf;

	switch (type) {
	case KEY:			s += "[KEY]"; break;
	case VALUE:			s += "[VALUE]"; break;
	case QUOTED:		s += "[QUOTED]"; break;
	case ASSIGN:		s += "[ASSIGN]"; break;
	case DOT:			s += "[DOT]"; break;
	case OPENBRACKET:	s += "[OPENBRACKET]"; break;
	case CLOSEBRACKET:	s += "[CLOSEBRACKET]"; break;
	case NEWLINE:		s += "[NEWLINE]"; break;
	case COMMENT:		s += "[COMMENT]"; break;
	case COMMA:			s += "[COMMA]"; break;
	case _EOF:			s += "[_EOF]"; break;
	case UNINITIALISED:
	default:			s += "[INVALID]"; break;
	}
	s += '\n';
	return s;
}

TokenParser::TokenParser(std::istream& input)
	: in(&input), pos(0), nc(0), last_token(Token::UNINITIALISED) {}

bool TokenParser::SkipSpace(std::string& line) {
	while (pos < line.size() && (line[pos] == ' ' || line[pos] == '\t')) pos++;
	return pos < line.size();
}

bool TokenParser::GetNextLine(std::string& line) {
	if (in->eof())
		return false;
	std::getline(*in, line);
	line.push_back('\n');
	nc++;
	pos = 0;
	last_token = Token::UNINITIALISED;
	return !in->fail();
}

void TokenParser::MakeToken(Token* t, const std::string& value, Token::e_token type) {
	t->value = value;
	t->type = type;
	t->line = nc;
	t->offset = pos;
}

Token::e_token TokenParser::Expect() const {
	switch (last_token) {
	case Token::UNINITIALISED:
		return Token::KEY | Token::COMMENT | Token::NEWLINE | Token::OPENBRACKET;
	case Token::KEY:
		return Token::ASSIGN | Token::DOT | Token::CLOSEBRACKET;
	case Token::ASSIGN:
		return Token::QUOTED | Token::KEY | Token::OPENBRACKET;
	case Token::QUOTED:
	case Token::VALUE:
		return Token::NEWLINE | Token::COMMA;
	case Token::DOT:
		return Token::KEY;
	case Token::OPENBRACKET:
		return Token::KEY | Token::QUOTED | Token::OPENBRACKET;
	case Token::CLOSEBRACKET:
		return Token::NEWLINE | Token::CLOSEBRACKET | Token::COMMENT;
	case Token::COMMENT:
		return Token::NEWLINE;
	case Token::COMMA:
		return Token::VALUE | Token::QUOTED;
	case Token::_EOF:
	case Token::NEWLINE:
	default:
		std::abort();
	}
}

Token::e_token TokenParser::ExpectValue() const {
	switch (last_token) {
	case Token::QUOTED:
	case Token::VALUE:
		return Token::NEWLINE | Token::COMMA | Token::COMMENT | Token::CLOSEBRACKET;
	case Token::OPENBRACKET:
		return Token::KEY | Token::QUOTED;
	case Token::CLOSEBRACKET:
		return Token::NEWLINE | Token::COMMENT;
	case Token::COMMENT:
		return Token::NEWLINE;
	case Token::COMMA:
		return Token::VALUE | Token::QUOTED;
	case Token::ASSIGN:
		return Token::OPENBRACKET | Token::QUOTED | Token::VALUE;
	default:
		return (Token::e_token)0;
	}
}

bool TokenParser::Tokenize(std::string& line, Token& tk) {
	if (!SkipSpace(line)) return false;

	char c = line[pos];
	switch (c) {
	case '.': MakeToken(&tk, ".", Token::DOT); pos++; break;
	case '[': MakeToken(&tk, "[", Token::OPENBRACKET); pos++; break;
	case ']': MakeToken(&tk, "]", Token::CLOSEBRACKET); pos++; break;
	case '=': MakeToken(&tk, "=", Token::ASSIGN); pos++; break;
	case '\n': MakeToken(&tk, "\n", Token::NEWLINE); pos++; break;
	case '"':
	case '\'': {
		size_t end = line.find(c, pos + 1);
		if (end == std::string::npos) return false;
		MakeToken(&tk, line.substr(pos + 1, end - pos - 1), Token::QUOTED);
		pos = end + 1;
		break;
	}
	case '#': {
		size_t end = line.find('\n', pos);
		if (end == std::string::npos) return false;
		MakeToken(&tk, line.substr(pos + 1, end), Token::COMMENT);
		pos = end + 1;
		break;
	}
	default:
		if (isalnum(c) || strchr("_-", c)) {
			size_t end = line.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-", pos);
			if (end == std::string::npos) return false;
			MakeToken(&tk, line.substr(pos, end - pos), Token::KEY);
			pos = end;
		} else return false;
		break;
	}
	return true;
}

bool TokenParser::TokenizeValue(std::string& line, Token& tk) {
	if (!SkipSpace(line)) return false;

	char c = line[pos];
	switch (c) {
	case '[': MakeToken(&tk, "[", Token::OPENBRACKET); pos++; break;
	case ']': MakeToken(&tk, "]", Token::CLOSEBRACKET); pos++; break;
	case ',': MakeToken(&tk, ",", Token::COMMA); pos++; break;
	case '"':
	case '\'': {
		size_t end = line.find(c, pos + 1);
		if (end == std::string::npos) return false;
		MakeToken(&tk, line.substr(pos + 1, end - pos - 1), Token::QUOTED);
		pos = end + 1;
		break;
	}
	default:
		if (isalnum(c) || strchr("_-", c)) {
			size_t end = line.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-", pos);
			if (end == std::string::npos) return false;
			MakeToken(&tk, line.substr(pos, end - pos), Token::VALUE);
			pos = end;
		} else return false;
		break;
	}
	return true;
}

TokenListResult TokenParser::ParseValue(std::string& line) {
	std::list<Token> tokens;
	Token tk;
	while (TokenizeValue(line, tk)) {
		if (!(tk.type & ExpectValue()))
			return ParseError("unexpected token `" + tk.asStr() + "`", nc);
		last_token = tk.type;
		tokens.push_back(tk);
	}
	return TokenListResult(tokens);
}

TokenListResult TokenParser::Parse() {
	std::list<Token> tokens;
	Token tk;
	while (GetNextLine(line)) {
		while (Tokenize(line, tk)) {
			if (!(tk.type & Expect()))
				return ParseError("unexpected token `" + tk.asStr() + "`", nc);
			last_token = tk.type;
			tokens.push_back(tk);
			if (last_token == Token::ASSIGN) {
				TokenListResult res = ParseValue(line);
				if (!res.isOk()) return res;
				tokens.splice(tokens.end(), res.Ok());
				break;
			}
		}
		MakeToken(&tk, "\n", Token::NEWLINE);
		tokens.push_back(tk);
	}
	return TokenListResult(tokens);
}
