#include "TomlTypedef.hpp"
#include "Parser.hpp"

bool isKey(TokenList::iterator& it, TokenList::iterator end) {
	while (it != end) {
		switch (it->type) {
		case Token::KEY:
			it++;
			if (it->type == Token::DOT)
				it++;
			break;
		case Token::CPAREN:
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
	if (!it->Is(Token::OPAREN))
		return false;
	it++;
	while (it != end) {
		switch (it->type) {
		case Token::VALUE:
		case Token::QUOTE:
		case Token::KEY:
			it++;
			if (it->type == Token::COMMA)
				it++;
			break;
		case Token::CPAREN:
			return it != end && it->Is(Token::CPAREN);
			break;
		default:
			return false;
			break;
		}
	}
	abort();
	return false;
}

ChekerResult SyntaxChecker(TokenList& tokens) {
	TokenList::iterator it = tokens.begin();
	TokenList::iterator end = tokens.end();

	while (it != end) {
		switch (it->type) {
		case Token::OPAREN: {
			it++; // skip OPAREN
			bool isArr = it->Is(Token::OPAREN);
			if (isArr)
				it++;
			if (!(isKey(it, end) && it->Is(Token::CPAREN)))
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
			if (!it->Is(Token::VALUE) && !it->Is(Token::QUOTE) && !isArr(it, end))
				return ChekerResult(ParseError("Invalid key [1002] ", it->line));
			it++; // skip VALUE QOUTED or CPAREN
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
