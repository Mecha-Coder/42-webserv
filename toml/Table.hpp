#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <vector>
#include <iostream>

// namespace toml {

// class Table {
// public:

// 	enum EToml {
// 		TABLE,
// 		ARRAY,
// 		STRING,
// 		NONE
// 	};

// 	typedef std::map<std::string, Table*> TomlMap;

// 	explicit Table(EToml type);
// 	Table(TomlMap& map);
// 	explicit Table(std::string& str);
//     Table(Table& other);
// 	~Table();

// 	Table& operator=(Table& other);
// 	Table& operator[](std::string& key);
// 	Table& operator[](std::size_t index);

// 	void Push(Table& t);
// 	void Insert(std::string& key, Table* value);
// 	void Create(std::string& key);
// 	void SetType(EToml type);
// 	void SetString(std::string& value);

// 	Table& Get(std::string& key);
// 	Table& Get(std::size_t index);
// 	Table& Last();

// 	bool isType(EToml type);
// 	const std::string& asStr(const std::string& fallback = "");
// 	int asInt(int fallback = 0);
// 	bool asBool(bool fallback = false);

// 	void Print(int indent = 0);

// 	static Table& Empty();

// private:
	
// 	EToml type;
// 	TomlMap map;
// 	std::vector<Table> vec;
// 	std::string str;
// };

// }

namespace toml {

class Table {
public:

	enum EToml {
		TABLE,
		ARRAY,
		STRING,
		NONE
	};

	typedef std::map<std::string, Table*> TomlMap;

	explicit Table(EToml type);
	Table(TomlMap& map);
	explicit Table(const std::string& str);
	Table(const Table& other);
	~Table();

	Table& operator=(const Table& other);
	Table& operator[](const std::string& key);
    const Table& operator[](const std::string& key) const;
	Table& operator[](std::size_t index);
    const Table& operator[](std::size_t index) const;

	void Push(const Table& t);
	void Insert(const std::string& key, Table* value);
	void Create(const std::string& key);
	void SetType(EToml type);
	void SetString(const std::string& value);

	Table& Get(const std::string& key);
    const Table& Get(const std::string& key) const;
	Table& Get(std::size_t index);
    const Table& Get(std::size_t index) const;
    const std::vector<Table>& GetVector() const;
    const TomlMap& GetMap() const;
	Table& Last();

	bool isType(EToml type) const;
	const std::string& asStr(const std::string& fallback = "") const;
	int asInt(int fallback = 0) const;
	bool asBool(bool fallback = false) const;

	void Print(int indent = 0) const;

	static Table& Empty();

private:
	EToml type;
	TomlMap map;
	std::vector<Table> vec;
	std::string str;
};

}
