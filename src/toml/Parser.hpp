#pragma once

#include "Table.hpp"
#include "ParseError.hpp"
#include "Result.hpp"
#include "Token.hpp"
#include <list>
#include <map>
#include <string>
#include <vector>

typedef std::list<Token> TokenList;
struct TokenPair;
typedef std::vector<TokenPair> TokenMap;

struct TokenPair {
	TokenList key;
	TokenList value;
	bool is_array;

	TokenPair(TokenList key, TokenList value, bool is_array = false) {
		this->key = key;
		this->value = value;
		this->is_array = is_array;
	}
};

struct TomlBlock {

	enum blockType { ARRAY, TABLE, ARRAY_VALUE };

	TomlBlock(TokenList prefix, TokenMap mp, enum blockType type) {
		this->mp = mp;
		this->prefix = prefix;
		this->type = type;
	}

	TokenList prefix;
	TokenMap mp;
	blockType type;
	std::vector<std::string> values;
};

class Parser {
public:
	TokenMap mp;
	std::vector<TomlBlock> tables;
	Parser(TokenList);
	void Print();
};

TokenList UntilIgnore(TokenList::iterator& cur, Token::e_token until, Token::e_token ignore);

TokenList::iterator Until(TokenList::iterator& cur, Token::e_token until);
typedef Result<toml::Table*, ParseError> ChekerResult;
ChekerResult SyntaxChecker(TokenList& tokens);
