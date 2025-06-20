/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Table.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaul <jpaul@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 21:10:24 by rcheong           #+#    #+#             */
/*   Updated: 2025/06/20 21:11:03 by jpaul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Table.hpp"
#include "../include/Utils.hpp"
#include <cstdlib>

using namespace toml;

Table::Table(EToml t) : type(t), str(""), line(-1) {}

Table::Table(TomlMap& m) : type(TABLE), map(m) {}

Table::Table(const std::string& s) : type(STRING), str(s), line(-1) {}

Table::Table(bool b) : type(STRING), str(b ? "true" : "false") {}

Table::Table(const Table& other) {
	type = other.type;
	str = other.str;
	vec = other.vec;

	for (TomlMap::const_iterator it = other.map.begin(); it != other.map.end(); ++it) {
		map[it->first] = new Table(*it->second);
	}
	line = other.line;
}

Table::~Table() {
	clearCurrentData();
}

void Table::clearCurrentData() {
	if (type == TABLE) {
		FOR_EACH(TomlMap, map, it) {
			if (it->second != NULL) {
				delete it->second;
				it->second = NULL;
			}
		}
		map.clear();
	}
	// Don't clear vec or str here as they might be needed
}

void Table::Push(const Table& t) {
	vec.push_back(t);
}

void Table::Insert(const std::string& s, Table* t) {
	if (type == ARRAY) {
		if (vec.empty())
			vec.push_back(Table(TABLE));
		vec.back().Insert(s, t);
	} else {
		map[s] = t;
	}
}

void Table::Create(const std::string& s) {
	Table* target = this;
	if (type == ARRAY) {
		if (vec.empty())
			vec.push_back(Table(TABLE));
		target = &vec.back();
	}
	if (!target->Get(s).isType(TABLE) && !target->Get(s).isType(ARRAY)) {
		target->Insert(s, new Table(TABLE));
	}
}

bool Table::isType(EToml t) const {
	return type == t;
}

void Table::setType(EToml t) {
	if (type == t) {
		return; // No change needed
	}
	
	// Only clear map data when converting away from TABLE
	if (type == TABLE && t != TABLE) {
		FOR_EACH(TomlMap, map, it) {
			if (it->second != NULL) {
				delete it->second;
				it->second = NULL;
			}
		}
		map.clear();
	}
	
	type = t;
}

void Table::setString(const std::string& s) {
	str = s;
}

Table& Table::Last() {
	if (!isType(ARRAY)) {
		return Empty();
	}
	if (vec.empty()) {
		vec.push_back(Table(TABLE));
	}
	return vec.back();
}

void Table::Print(int indent) const {
	std::string pad(indent, ' ');
	switch (type) {
	case STRING:
		std::cout << pad << str << std::endl;
		break;
	case TABLE:
		std::cout << pad << "{" << std::endl;
		for (TomlMap::const_iterator it = map.begin(); it != map.end(); ++it) {
			std::cout << pad << "  " << it->first << ": ";
			it->second->Print(indent + 2);
		}
		std::cout << pad << "}" << std::endl;
		break;
	case ARRAY:
		std::cout << pad << "[" << std::endl;
		for (std::vector<Table>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
			it->Print(indent + 2);
		}
		std::cout << pad << "]" << std::endl;
		break;
	case NONE:
		std::cout << pad << "NONE" << std::endl;
		break;
	}
}

Table& Table::operator[](const std::string& index) {
	return Get(index);
}

const Table& Table::operator[](const std::string& index) const {
	return Get(index);
}

Table& Table::operator[](std::size_t index) {
	return Get(index);
}

const Table& Table::operator[](std::size_t index) const {
	return Get(index);
}

Table& Table::operator=(const Table& other) {
	if (this == &other)
		return *this;

	clearCurrentData();

	this->type = other.type;
	this->str = other.str;

	for (TomlMap::const_iterator mit = other.map.begin(); mit != other.map.end(); ++mit) {
		this->map[mit->first] = new Table(*mit->second);
	}

	this->vec = other.vec;
	this->line = other.line;

	return *this;
}

const std::string& Table::asStr(const std::string& fallback) const {
	return type == STRING ? str : fallback;
}

int Table::asInt(int fallback) const {
	if (type != STRING) return fallback;
	char* end;
	long val = std::strtol(str.c_str(), &end, 10);
	return (*end == '\0') ? static_cast<int>(val) : fallback;
}

bool Table::asBool(bool fallback) const {
	if (type != STRING) return fallback;
	if (str == "true" || str == "on" || str == "yes") return true;
	if (str == "false" || str == "off" || str == "no") return false;
	return fallback;
}

Table& Table::Get(const std::string& s) {
	if (!isType(TABLE)) return Empty();
	TomlMap::iterator it = map.find(s);
	return (it != map.end()) ? *(it->second) : Empty();
}

const Table& Table::Get(const std::string& key) const {
	if (!isType(TABLE)) return Empty();
	TomlMap::const_iterator it = map.find(key);
	return (it != map.end()) ? *(it->second) : Empty();
}

Table& Table::Get(std::size_t index) {
	if (!isType(ARRAY) || index >= vec.size()) return Empty();
	return vec[index];
}

const Table& Table::Get(std::size_t index) const {
	if (!isType(ARRAY) || index >= vec.size()) return Empty();
	return vec[index];
}

const std::vector<Table>& Table::getVector() const {
	return vec;
}

const Table::TomlMap& Table::getMap() const {
	return map;
}

std::string Table::getTypeName() const {
	switch (type) {
		case TABLE: return "TABLE";
		case ARRAY: return "ARRAY";
		case STRING: return "STRING";
		case NONE: return "NONE";
		default: return "UNKNOWN";
	}
}

std::vector<std::string> Table::getKeys() const {
	std::vector<std::string> keys;
	if (!isType(TABLE)) return keys;
	for (TomlMap::const_iterator it = map.begin(); it != map.end(); ++it) {
		keys.push_back(it->first);
	}
	return keys;
}

bool Table::isValid() const {
	if (type == TABLE)
		return !map.empty();
	if (type == ARRAY)
		return !vec.empty();
	if (type == STRING)
		return !str.empty();
	return false;
}

std::size_t Table::Size() const {
	if (type == TABLE)
		return map.size();
	if (type == ARRAY)
		return vec.size();
	if (type == STRING)
		return 1;
	return 0;
}

void Table::Remove(const std::string& key) {
	TomlMap::iterator it = map.find(key);
	if (it != map.end()) {
		delete it->second;
		map.erase(it);
	}
}

Table& Table::Empty() {
	static Table emptyTable(NONE);
	return emptyTable;
}
