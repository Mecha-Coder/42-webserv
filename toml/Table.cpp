#include "Table.hpp"
#include "../utils/Utils.hpp"
#include <cstdlib>

using namespace toml;

// /**
//  * @brief constructor for the table class.
//  * @param type The type of the table (TABLE, ARRAY, STRING, NONE).
//  * @details Init table with the specified type.
//  */
// Table::Table(EToml t) : type(t), str("") {}

// Table::Table(TomlMap& m) : type(TABLE), map(m) {}

// Table::Table(std::string& s) : type(STRING), str(s) {}

// Table::Table(Table& other) {
// 	type = other.type;
// 	str = other.str;
//     vec = other.vec;

// 	for (auto& pair : other.map) {
// 		map[pair.first] = new Table(*pair.second);
// 	}
// 	vec = other.vec;
// }

// /**
//  * @brief Destructor for the table class.
//  * @details Cleans up the map by deleting all dynamically allocated tables.
//  */
// Table::~Table() {
// 	FOR_EACH(TomlMap, map, it) {
// 		if (it->second != nullptr) {
// 			delete it->second;
// 			it->second = nullptr;
// 		}
// 	}
// }

// /**
//  * @brief Adds new table to vector, grouped in array.
//  */
// void Table::Push(Table& t) {
// 	vec.push_back(t);
// }

// void Table::Insert(std::string& s, Table* t) {
// 	if (t->isType(ARRAY)) {
// 		if (vec.empty())
// 			vec.push_back(Table(TABLE));
// 		vec.back().Insert(s, t);
// 	} else {
// 		map[s] = t;
// 	}
// }

// void Table::Create(std::string& s) {
// 	Table* target = this;
// 	if (isType(ARRAY)) {
// 		if (vec.empty())
// 			vec.push_back(Table(TABLE));
// 		target = &vec.back();
// 	}
// 	if (!target->Get(s).isType(TABLE)) {
// 		target->Insert(s, new Table(TABLE));
// 	}
// }

// bool Table::isType(EToml t) {
// 	return type == t;
// }

// void Table::SetType(EToml t) {
// 	type = t;
// }

// void Table::SetString(std::string& s) {
// 	str = s;
// }

// Table& Table::Last() {
// 	if (!isType(ARRAY)) {
// 		return Empty();
// 	}
// 	if (vec.empty()) {
// 		vec.push_back(Table(TABLE));
// 	}
// 	return vec.back();
// }

// void Table::Print(int indent) {
// 	std::string pad(indent, ' ');
// 	switch (type) {
// 	case STRING:
// 		std::cout << pad << str << std::endl;
// 		break;
// 	case TABLE:
// 		std::cout << pad << "{" << std::endl;
// 		FOR_EACH(TomlMap, map, it) {
// 			std::cout << pad << "  " << it->first << ": ";
// 			it->second->Print(indent + 2);
// 		}
// 		std::cout << pad << "}" << std::endl;
// 		break;
// 	case ARRAY:
// 		std::cout << pad << "[" << std::endl;
// 		FOR_EACH(std::vector<Table>, vec, it) {
// 			it->Print(indent + 2);
// 		}
// 		std::cout << pad << "]" << std::endl;
// 		break;
// 	case NONE:
// 		std::cout << pad << "NONE" << std::endl;
// 		break;
// 	}
// }

// Table& Table::operator[](std::string& index) {
// 	return Get(index);
// }

// Table& Table::operator[](std::size_t index) {
// 	return Get(index);
// }

// toml::Table& toml::Table::operator=(Table& other) {
// 	if (this == &other)
// 		return *this;

// 	TomlMap::iterator it = map.begin();
// 	while (it != map.end()) {
// 		delete it->second;
// 		++it;
// 	}
// 	map.clear();
// 	vec.clear();
// 	str.clear();

// 	this->type = other.type;
// 	this->str = other.str;

// 	FOR_EACH_CONST(Table::TomlMap, other.map, mit) {
// 		this->map[mit->first] = new Table(*mit->second);
// 	}

// 	this->vec = other.vec;

// 	return *this;
// }

// const std::string& Table::asStr(const std::string& fallback) {
// 	return type == STRING ? str : fallback;
// }

// int Table::asInt(int fallback) {
// 	if (type != STRING) return fallback;
// 	char* end;
// 	long val = std::strtol(str.c_str(), &end, 10);
// 	return (*end == '\0') ? static_cast<int>(val) : fallback;
// }

// bool Table::asBool(bool fallback) {
// 	if (type != STRING) return fallback;
// 	if (str == "true" || str == "on" || str == "yes") return true;
// 	if (str == "false" || str == "off" || str == "no") return false;
// 	return fallback;
// }

// Table& Table::Get(std::string& s) {
// 	if (!isType(TABLE)) return Empty();
// 	TomlMap::iterator it = map.find(s);
// 	return (it != map.end()) ? *(it->second) : Empty();
// }

// Table& Table::Get(std::size_t index) {
// 	if (!isType(ARRAY) || index >= vec.size()) return Empty();
// 	return vec[index];
// }

// Table& Table::Empty() {
// 	static Table emptyTable(NONE);
// 	return emptyTable;
// }

Table::Table(EToml t) : type(t), str("") {}

Table::Table(TomlMap& m) : type(TABLE), map(m) {}

Table::Table(const std::string& s) : type(STRING), str(s) {}

Table::Table(const Table& other) {
	type = other.type;
	str = other.str;
	vec = other.vec;

	for (TomlMap::const_iterator it = other.map.begin(); it != other.map.end(); ++it) {
		map[it->first] = new Table(*it->second);
	}
}

Table::~Table() {
	FOR_EACH(TomlMap, map, it) {
		if (it->second != nullptr) {
			delete it->second;
			it->second = nullptr;
		}
	}
}

void Table::Push(const Table& t) {
	vec.push_back(t);
}

void Table::Insert(const std::string& s, Table* t) {
	if (t->isType(ARRAY)) {
		if (vec.empty())
			vec.push_back(Table(TABLE));
		vec.back().Insert(s, t);
	} else {
		map[s] = t;
	}
}

void Table::Create(const std::string& s) {
	Table* target = this;
	if (isType(ARRAY)) {
		if (vec.empty())
			vec.push_back(Table(TABLE));
		target = &vec.back();
	}
	if (!target->Get(s).isType(TABLE)) {
		target->Insert(s, new Table(TABLE));
	}
}

bool Table::isType(EToml t) const {
	return type == t;
}

void Table::SetType(EToml t) {
	type = t;
}

void Table::SetString(const std::string& s) {
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

	TomlMap::iterator it = map.begin();
	while (it != map.end()) {
		delete it->second;
		++it;
	}
	map.clear();
	vec.clear();
	str.clear();

	this->type = other.type;
	this->str = other.str;

	for (TomlMap::const_iterator mit = other.map.begin(); mit != other.map.end(); ++mit) {
		this->map[mit->first] = new Table(*mit->second);
	}

	this->vec = other.vec;

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

const std::vector<Table>& Table::GetVector() const {
	return vec;
}

const Table::TomlMap& Table::GetMap() const {
	return map;
}

Table& Table::Empty() {
	static Table emptyTable(NONE);
	return emptyTable;
}
