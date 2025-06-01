#include "Toml.hpp"
#include "Table.hpp"
#include "ParseError.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include "Token.hpp"
#include "../utils/Utils.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <istream>
#include <list>
#include <string>
#include <vector>

using namespace toml;

template <typename Iter>
Iter next_it(Iter iter) {
	return ++iter;
}

Table& list2map(TokenList list, Table& t) {
	Table* last_t = &t;

	FOR_EACH(TokenList, list, it) {
		last_t->Create(it->value);
		last_t = &last_t->Get(it->value);
		if (next_it(it) != list.end() && last_t->type == Table::ARRAY) {
			last_t = &last_t->Get(last_t->vec.size() - 1);
		}
	}
	return *last_t;
}

void fill_map(TokenMap& mp, Table& t) {
	FOR_EACH(TokenMap, mp, it) {
		Table& last = list2map(it->key, t);
		if (it->is_array) {
			last.setType(Table::ARRAY);
			FOR_EACH(TokenList, it->value, it2) {
				last.vec.push_back(it2->value);
			}
			continue;
		}
		last.setType(Table::STRING);
		last.setString(it->value.front().value);
	}
}

Table* build(Parser& p) {
	Table* t = new Table(Table::TABLE);

	fill_map(p.mp, *t);

	FOR_EACH(std::vector<TomlBlock>, p.Table, it) {
		if (it->type == TomlBlock::TABLE) {
			Table& last = list2map(it->prefix, *t);
			if (last.type == Table::ARRAY) {
				last = last.Get(last.vec.size() - 1);
			}
			fill_map(it->mp, last);
			continue;
		}
		Table tmp = Table(Table::TABLE);
		Table& last = list2map(it->prefix, *t);
		last.setType(Table::ARRAY);
		fill_map(it->mp, tmp);
		last.Push(tmp);
	}

	return t;
}

Table* toml::parse_stream(std::ifstream& in) {
	Lexer lexer = Lexer(in);
	TokenListResult resToken = lexer.parse();
	if (!resToken.is_ok()) {
		cerr << resToken.err().as_str() << endl;
		return NULL;
	}

	TokenList tokens = resToken.ok();
	ChekerResult res = syntax_checker(tokens);
	if (!res.is_ok()) {
		cerr << res.err().as_str() << endl;
		return NULL;
	}
	Parser p = Parser(tokens);

	return build(p);
}

Table* toml::parse_file(std::string& filename) {
	std::ifstream ifs(filename);

	if (!ifs.is_open())
		return NULL;
	return parse_stream(ifs);
}
