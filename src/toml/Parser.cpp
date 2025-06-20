#include "Parser.hpp"
#include "../include/Utils.hpp"
#include <algorithm>
#include <iostream>
#include <list>
#include <numeric>
#include <string>

TokenList UntilIgnore(TokenList::iterator& cur, Token::e_token until, Token::e_token ignore) {
	TokenList res;
	while (!cur->Is(until)) {
		if (!cur->Is(ignore)) {
			res.push_back(*cur);
		}
		cur++;
	}
	return res;
}

TokenList::iterator Until(TokenList::iterator& cur, Token::e_token until) {
	while (!cur->Is(until)) {
		cur++;
	}
	return cur;
}

std::string Accumulate(std::string& acc, Token& t) {
	return acc + t.value;
}

Parser::Parser(TokenList tks) {
	TokenList::iterator cur = tks.begin();
	TokenMap* lastmp = &mp;

	while (cur != tks.end()) {
		switch (cur->type) {
		case Token::KEY: {
			TokenList res = UntilIgnore(cur, Token::ASSIGN, Token::DOT);
			cur++; // skip assign
			bool is_array = cur->Is(Token::OPAREN);
			TokenList values = UntilIgnore(cur,
										  Token::NEWLINE | Token::COMMENT,
										  Token::COMMA | Token::CPAREN | Token::OPAREN);
			lastmp->push_back(TokenPair(res, values, is_array));

			//TODO: debug print
			// if (!res.empty()) {
			// 	std::cout << "[Parser] Inserted key = ";
			// 	FOR_EACH(TokenList, res, tok)
			// 		std::cout << tok->value << ".";
			// 	std::cout << (is_array ? " (array)" : " (scalar)") << std::endl;
			// }

			break;
		}
		case Token::OPAREN: {
			cur++;
			bool is_array = cur->Is(Token::OPAREN);
			std::vector<TomlBlock>* tm = &this->tables;
			TomlBlock::blockType type = TomlBlock::TABLE;
			if (is_array) {
				type = TomlBlock::ARRAY;
				cur++;
			}
			TokenList res = UntilIgnore(cur, Token::CPAREN | Token::COMMENT, Token::DOT);
			tm->push_back(TomlBlock(res, TokenMap(), type));
			lastmp = &tm->back().mp;

			// TODO: debug print
			// std::cout << "[PARSER] type=" << (type == TomlBlock::ARRAY ? "ARRAY" : "TABLE") << " → ";
			// FOR_EACH(TokenList, res, tok) {
			// 	std::cout << tok->value << ".";
			// }
			// std::cout << std::endl;

			break;
		}
		default:
			break;
		}
		cur++;
	}
}

void PrintKeyValue(TokenMap& mp) {
	FOR_EACH(TokenMap, mp, m) {
		FOR_EACH_CONST(TokenList, m->key, it) {
			std::cout << it->value << " ";
		}
		std::cout << " = ";
		if (m->is_array) {
			std::cout << "[";
			FOR_EACH_CONST(TokenList, m->value, it) {
				std::cout << it->value << ", ";
			}
			std::cout << "]";
			continue;
		}
		std::cout << m->value.front().value;
		std::cout << std::endl;
	}
}

void PrintList(TokenList& list) {
	TokenList::iterator begin = list.begin();
	while (begin != list.end()) {
		std::cout << begin->value << ">";
		begin++;
	}
	std::cout << std::endl;
}

void PrintBlock(std::vector<TomlBlock>& vec) {

	std::cout << "================================================" << std::endl;
	FOR_EACH(std::vector<TomlBlock>, vec, block) {
		PrintList(block->prefix);
		PrintKeyValue(block->mp);
	}
}

void Parser::Print() {
	PrintKeyValue(mp);
	PrintBlock(tables);
}
