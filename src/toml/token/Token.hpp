#include "Result.hpp"
#include "ParseError.hpp"
#include <istream>
#include <list>
#include <string>

class Token {
public:
	enum e_token {
		UNINITIALISED	= 1,
		KEY				= 1 << 1,
		ASSIGN			= 1 << 2,
		QUOTED			= 1 << 3,
		DOT				= 1 << 4,
		OPENBRACKET		= 1 << 5,
		CLOSEBRACKET	= 1 << 6,
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
	bool is(e_token t) const;
	std::string as_str() const;
};

inline Token::e_token operator|(Token::e_token t1, Token::e_token t2) {
	return (Token::e_token)((int)t1 | (int)t2);
}

typedef Result<std::list<Token>, ParseError> TokenListResult;

class TokenParser {
public:
	TokenParser(std::istream& input);
	TokenListResult parse();

private:
	std::istream* in;
	std::string line;
	size_t pos;
	size_t nc;
	Token::e_token last_token;

	bool skip_space(std::string& line);
	bool get_next_line(std::string& line);
	void make_token(Token* t, const std::string& value, Token::e_token type);
	Token::e_token expect() const;
	Token::e_token expect_value() const;

	bool tokenize(std::string& line, Token& tk);
	bool tokenize_value(std::string& line, Token& tk);
	TokenListResult parse_value(std::string& line);
};
