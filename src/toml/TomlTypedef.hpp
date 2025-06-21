#pragma once

#include <list>

class Token;
class ParseError;

template <typename SuccessType, typename ErrorType>
class Result;

typedef std::list<Token> TokenList;
typedef Result<TokenList, ParseError> TokenListResult;

inline bool operator==(const Token& a, const Token& b) {
	return a.type == b.type && a.value == b.value;
}
