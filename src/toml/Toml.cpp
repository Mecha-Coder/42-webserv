#include "Toml.hpp"
#include "TomlTypedef.hpp"
#include "Table.hpp"
#include "ParseError.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include "Token.hpp"
#include "../include/Utils.hpp"
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
			// std::cout << "[List2Map] Accessing key: " << it->value << " in type: " << last_t->getTypeName() << std::endl;
			
			// for arrays, access the last element if it exists
			if (!last_t->vec.empty()) {
				last_t = &last_t->Get(last_t->vec.size() - 1);
			}
		}
	}
	return *last_t;
}

void FillMap(TokenMap& mp, Table& t) {
	FOR_EACH(TokenMap, mp, it) {
		Table& last = List2Map(it->key, t);

		if (it->is_array) {
			last.setType(Table::ARRAY);
			last.line = it->value.empty() ? -1 : it->value.front().line;
			FOR_EACH(TokenList, it->value, it2) {
				Table entry(it2->value);
				entry.line = it2->line;
				last.vec.push_back(entry);
			}
			continue;
		}

		//TODO: debug print
		// std::cout << "[FillMap] Inserting flat key = ";
		// FOR_EACH(TokenList, it->key, tok)
		// 	std::cout << tok->value << ".";
		// std::cout << " is_array=" << it->is_array;
		// if (last.isValid())
		// 	std::cout << " (existing type: " << last.getTypeName() << ")";
		// std::cout << std::endl;

		last.setType(Table::STRING);
		last.setString(it->value.front().value);
		last.line = it->value.front().line;
	}
}

Table* Build(Parser& p) {
	Table* t = new Table(Table::TABLE);

	FillMap(p.mp, *t);

	//TODO: debug print
	// std::cout << "[Build] Starting Build()" << std::endl;

	FOR_EACH(std::vector<TomlBlock>, p.tables, it) {

		//TODO: debug print
		// std::cout << "[Build] Block prefix = ";
		// FOR_EACH(TokenList, it->prefix, tok)
		// 	std::cout << tok->value << ".";
		// std::cout << " type = " << (it->type == TomlBlock::ARRAY ? "ARRAY" : "TABLE") << std::endl;

		if (it->type == TomlBlock::TABLE) {
			Table& last = List2Map(it->prefix, *t);
			if (last.type == Table::ARRAY && !last.vec.empty()) {
				last = last.Get(last.vec.size() - 1);
			}
			FillMap(it->mp, last);
			continue;
		}
		
		// Handle ARRAY type blocks
		Table tmp = Table(Table::TABLE);
		tmp.line = it->prefix.front().line;
		FillMap(it->mp, tmp);
		
		// Navigate to the target location
		Table* target = t;
		TokenList::iterator prefix_it = it->prefix.begin();
		
		// Navigate to parent containers
		while (NextIter(prefix_it) != it->prefix.end()) {
			target->Create(prefix_it->value);
			target = &target->Get(prefix_it->value);
			if (target->type == Table::ARRAY && !target->vec.empty()) {
				target = &target->Get(target->vec.size() - 1);
			}
			++prefix_it;
		}
		
		// Handle the final key (e.g., "server")
		std::string final_key = prefix_it->value;
		
		// Check if this key already exists
		Table& final_target = target->Get(final_key);
		if (final_target.isType(Table::NONE)) {
			// First time seeing this key - create as ARRAY immediately
			target->Insert(final_key, new Table(Table::ARRAY));
			target->Get(final_key).line = prefix_it->line;

			// TODO: debug print
			// std::cout << "[Build] Creating new ARRAY for " << final_key << std::endl;

		} else if (!final_target.isType(Table::ARRAY)) {
			// Convert existing to ARRAY - but preserve any existing data

			// TODO: debug print
			// std::cout << "[Build] Converting to ARRAY for ";
			// FOR_EACH(TokenList, it->prefix, tok)
			// 	std::cout << tok->value << ".";
			// std::cout << std::endl;
			
			// Save existing data if it's a TABLE
			if (final_target.isType(Table::TABLE)) {
				Table old_table = final_target; // Copy constructor
				final_target.setType(Table::ARRAY);
				final_target.vec.push_back(old_table);
			} else {
				final_target.setType(Table::ARRAY);
			}
		}
		
		// Add the new entry to the array
		target->Get(final_key).Push(tmp);
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
