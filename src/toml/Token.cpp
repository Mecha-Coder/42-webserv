#include "Token.hpp"
#include <cctype>
#include <cstdlib>
#include <string.h>

Token::Token() : type(UNINITIALISED), line(0), offset(0) {}

bool Token::is(e_token t) const {
	return (type & t) != 0;
}

std::string Token::as_str() const {
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

bool TokenParser::skip_space(std::string& line) {
	while (pos < line.size() && (line[pos] == ' ' || line[pos] == '\t')) pos++;
	return pos < line.size();
}

bool TokenParser::get_next_line(std::string& line) {
	if (in->eof()) return false;
	std::getline(*in, line);
	line.push_back('\n');
	nc++;
	pos = 0;
	last_token = Token::UNINITIALISED;
	return !in->fail();
}

void TokenParser::make_token(Token* t, const std::string& value, Token::e_token type) {
	t->value = value;
	t->type = type;
	t->line = nc;
	t->offset = pos;
}

Token::e_token TokenParser::expect() const {
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

Token::e_token TokenParser::expect_value() const {
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

bool TokenParser::tokenize(std::string& line, Token& tk) {
	if (!skip_space(line)) return false;

	char c = line[pos];
	switch (c) {
	case '.': make_token(&tk, ".", Token::DOT); pos++; break;
	case '[': make_token(&tk, "[", Token::OPENBRACKET); pos++; break;
	case ']': make_token(&tk, "]", Token::CLOSEBRACKET); pos++; break;
	case '=': make_token(&tk, "=", Token::ASSIGN); pos++; break;
	case '\n': make_token(&tk, "\n", Token::NEWLINE); pos++; break;
	case '"':
	case '\'': {
		size_t end = line.find(c, pos + 1);
		if (end == std::string::npos) return false;
		make_token(&tk, line.substr(pos + 1, end - pos - 1), Token::QUOTED);
		pos = end + 1;
		break;
	}
	case '#': {
		size_t end = line.find('\n', pos);
		if (end == std::string::npos) return false;
		make_token(&tk, line.substr(pos + 1, end), Token::COMMENT);
		pos = end + 1;
		break;
	}
	default:
		if (isalnum(c) || strchr("_-", c)) {
			size_t end = line.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-", pos);
			if (end == std::string::npos) return false;
			make_token(&tk, line.substr(pos, end - pos), Token::KEY);
			pos = end;
		} else return false;
		break;
	}
	return true;
}

bool TokenParser::tokenize_value(std::string& line, Token& tk) {
	if (!skip_space(line)) return false;

	char c = line[pos];
	switch (c) {
	case '[': make_token(&tk, "[", Token::OPENBRACKET); pos++; break;
	case ']': make_token(&tk, "]", Token::CLOSEBRACKET); pos++; break;
	case ',': make_token(&tk, ",", Token::COMMA); pos++; break;
	case '"':
	case '\'': {
		size_t end = line.find(c, pos + 1);
		if (end == std::string::npos) return false;
		make_token(&tk, line.substr(pos + 1, end - pos - 1), Token::QUOTED);
		pos = end + 1;
		break;
	}
	default:
		if (isalnum(c) || strchr("_-", c)) {
			size_t end = line.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-", pos);
			if (end == std::string::npos) return false;
			make_token(&tk, line.substr(pos, end - pos), Token::VALUE);
			pos = end;
		} else return false;
		break;
	}
	return true;
}

TokenListResult TokenParser::parse_value(std::string& line) {
	std::list<Token> tokens;
	Token tk;
	while (tokenize_value(line, tk)) {
		if (!(tk.type & expect_value()))
			return ParseError("unexpected token `" + tk.as_str() + "`", nc);
		last_token = tk.type;
		tokens.push_back(tk);
	}
	return TokenListResult(tokens);
}

TokenListResult TokenParser::parse() {
	std::list<Token> tokens;
	Token tk;
	while (get_next_line(line)) {
		while (tokenize(line, tk)) {
			if (!(tk.type & expect()))
				return ParseError("unexpected token `" + tk.as_str() + "`", nc);
			last_token = tk.type;
			tokens.push_back(tk);
			if (last_token == Token::ASSIGN) {
				TokenListResult res = parse_value(line);
				if (!res.is_ok()) return res;
				tokens.splice(tokens.end(), res.ok());
				break;
			}
		}
		make_token(&tk, "\n", Token::NEWLINE);
		tokens.push_back(tk);
	}
	return TokenListResult(tokens);
}
