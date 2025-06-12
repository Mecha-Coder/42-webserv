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

			// TODO: debug print
			std::cout << "[List2Map] Accessing key: " << it->value << " in type: " << last_t->getTypeName() << std::endl;
			
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

		//TODO: debug print
		std::cout << "[FillMap] Inserting flat key = ";
		FOR_EACH(TokenList, it->key, tok)
			std::cout << tok->value << ".";
		std::cout << " is_array=" << it->is_array;

		if (last.isValid())
			std::cout << " (existing type: " << last.getTypeName() << ")";
		std::cout << std::endl;

		last.setType(Table::STRING);
		last.setString(it->value.front().value);
	}
}

struct MapPath {
	Table* parent;
	std::string key;
};

/**
 * @brief Resolves a list of keys to the parent Table and final key.
 * @details This allows Build() to modify the final entry (e.g., "server") from TABLE to ARRAY.
 */
MapPath ResolvePath(TokenList list, Table& root) {
	Table* cur = &root;
	TokenList::iterator it = list.begin();
	while (NextIter(it) != list.end()) {
		cur->Create(it->value);
		cur = &cur->Get(it->value);
		++it;
	}
	MapPath result;
	result.parent = cur;
	result.key = it->value;
	return result;
}

Table* Build(Parser& p) {
	Table* t = new Table(Table::TABLE);

	FillMap(p.mp, *t);

	//TODO: debug print
	std::cout << "[Build] Starting Build()" << std::endl;

	FOR_EACH(std::vector<TomlBlock>, p.tables, it) {

		//TODO: debug print
		std::cout << "[Build] Block prefix = ";
		FOR_EACH(TokenList, it->prefix, tok)
			std::cout << tok->value << ".";
		std::cout << " type = " << (it->type == TomlBlock::ARRAY ? "ARRAY" : "TABLE") << std::endl;

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

		// TODO: debug print
		if (!last.isType(Table::ARRAY)) {
			if (last.isType(Table::TABLE)) {
				std::cout << "[Build] Upgrading TABLE to ARRAY for ";
				for (TokenList::iterator tk = it->prefix.begin(); tk != it->prefix.end(); ++tk)
					std::cout << tk->value << ".";
				std::cout << std::endl;

			Table old = last;
			last.setType(Table::ARRAY);
			last.vec.clear();
			last.vec.push_back(old);
			} else {
			last.setType(Table::ARRAY);
			}
		}
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

	if (!ifs.is_open()) {
		std::cerr << "[Toml] Failed to open file: " << filename << std::endl;
		return NULL;
	}
	return ParseStream(ifs);
}
