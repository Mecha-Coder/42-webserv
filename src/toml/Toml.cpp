#include "Toml.hpp"
#include "TomlTypedef.hpp"
#include "Table.hpp"
#include "ParseError.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include "Token.hpp"
// #include "Result.hpp"
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
Iter NextIter(Iter iter) {
	return ++iter;
}

Table& List2Map(TokenList list, Table& t) {
	Table* last_t = &t;

	FOR_EACH(TokenList, list, it) {
		last_t->Create(it->value);
		last_t = &last_t->Get(it->value);
		if (NextIter(it) != list.end() && last_t->type == Table::ARRAY) {
			last_t = &last_t->Get(last_t->vec.size() - 1);
		}
	}
	return *last_t;
}

void FillMap(TokenMap& mp, Table& t) {
	FOR_EACH(TokenMap, mp, it) {
		Table& last = List2Map(it->key, t);
		if (it->is_array) {
			last.setType(Table::ARRAY);
			FOR_EACH(TokenList, it->value, it2) {
				last.vec.push_back(Table(it2->value));
			}
			continue;
		}
		last.setType(Table::STRING);
		last.setString(it->value.front().value);
	}
}

Table* Build(Parser& p) {
	Table* t = new Table(Table::TABLE);

	FillMap(p.mp, *t);

	FOR_EACH(std::vector<TomlBlock>, p.tables, it) {
		if (it->type == TomlBlock::TABLE) {
			Table& last = List2Map(it->prefix, *t);
			if (last.type == Table::ARRAY) {
				last = last.Get(last.vec.size() - 1);
			}
			FillMap(it->mp, last);
			continue;
		}
		Table tmp = Table(Table::TABLE);
		Table& last = List2Map(it->prefix, *t);
		last.setType(Table::ARRAY);
		FillMap(it->mp, tmp);
		last.Push(tmp);
	}

	return t;
}

Table* toml::ParseStream(std::ifstream& in) {
	Lexer lexer = Lexer(in);
	TokenListResult resToken = lexer.Parse();
	if (!resToken.isOk()) {
		std::cerr << resToken.Err().asStr() << endl;
		return NULL;
	}

	TokenList tokens = resToken.Ok();
	ChekerResult res = SyntaxChecker(tokens);
	if (!res.isOk()) {
		cerr << res.Err().asStr() << endl;
		return NULL;
	}
	Parser p = Parser(tokens);

	return Build(p);
}

Table* toml::ParseFile(std::string& filename) {
	std::ifstream ifs(filename.c_str());

	if (!ifs.is_open())
		return NULL;
	return ParseStream(ifs);
}
