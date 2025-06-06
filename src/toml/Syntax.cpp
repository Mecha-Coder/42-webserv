#include "Parser.hpp"

bool isKey(TokenList::iterator& it, TokenList::iterator end) {
	while (it != end) {
		switch (it->type) {
		case Token::KEY:
			it++;
			if (it->type == Token::DOT)
				it++;
			break;
		case Token::CLOSEBRACKET:
		case Token::ASSIGN:
			return true;
			break;
		default:
			return false;
			break;
		}
	}
	return true;
}

bool isArr(TokenList::iterator& it, TokenList::iterator end) {
	if (!it->Is(Token::OPENBRACKET))
		return false;
	it++;
	while (it != end) {
		switch (it->type) {
		case Token::VALUE:
		case Token::QUOTED:
		case Token::KEY:
			it++;
			if (it->type == Token::COMMA)
				it++;
			break;
		case Token::CLOSEBRACKET:
			return it != end && it->Is(Token::CLOSEBRACKET);
			break;
		default:
			return false;
			break;
		}
	}
	abort(); // should never reach here
	return false;
}

ChekerResult SyntaxChecker(TokenList& tokens) {
	TokenList::iterator it = tokens.begin();
	TokenList::iterator end = tokens.end();

	while (it != end) {
		switch (it->type) {
		case Token::OPENBRACKET: {
			it++; // skip OPENBRACKET
			bool isArr = it->Is(Token::OPENBRACKET);
			if (isArr)
				it++;
			if (!(isKey(it, end) && it->Is(Token::CLOSEBRACKET)))
				return ChekerResult(ParseError("Invalid key [1000] ", it->line));
			;
			if (isArr)
				it++;
			it++;
			break;
		}
		case Token::KEY: {
			if (!(isKey(it, end) && it->Is(Token::ASSIGN)))
				return ChekerResult(ParseError("Invalid key [1001] ", it->line));
			it++; // skip ASSIGN
			if (!it->Is(Token::VALUE) && !it->Is(Token::QUOTED) && !isArr(it, end))
				return ChekerResult(ParseError("Invalid key [1002] ", it->line));
			it++; // skip VALUE QOUTED or CLOSEBRACKET
			break;
		}
		case Token::NEWLINE:
		case Token::COMMENT:
			break;
		case Token::_EOF:
			return ChekerResult(NULL);
			break;
		default:
			return ChekerResult(ParseError("Invalid key [1003] ", it->line));
			break;
		}
		if (it->Is(Token::COMMENT))
			it++;
		if (!it->Is(Token::NEWLINE) && !it->Is(Token::_EOF))
			return ChekerResult(ParseError("Invalid key [1004] ", it->line));
		++it;
	}
	return ChekerResult(NULL);
}
