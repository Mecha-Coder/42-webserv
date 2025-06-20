/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Table.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcheong <rcheong@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 21:10:22 by rcheong           #+#    #+#             */
/*   Updated: 2025/06/11 21:56:14 by rcheong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <vector>
#include <iostream>

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
	explicit Table(bool b);
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
	void setType(EToml type);
	void setString(const std::string& value);

	Table& Get(const std::string& key);
	const Table& Get(const std::string& key) const;
	Table& Get(std::size_t index);
	const Table& Get(std::size_t index) const;
	const std::vector<Table>& getVector() const;
	const TomlMap& getMap() const;
	Table& Last();

	bool isType(EToml type) const;
	const std::string& asStr(const std::string& fallback = "") const;
	int asInt(int fallback = 0) const;
	bool asBool(bool fallback = false) const;

	void Print(int indent = 0) const;
	bool isValid() const;
	std::string getTypeName() const;
	std::vector<std::string> getKeys() const;
	std::size_t Size() const;

	void Remove(const std::string& key);
	static Table& Empty();

// private:
	enum EToml type;
	TomlMap map;
	std::vector<Table> vec;
	std::string str;

private:
	void clearCurrentData();
};

}
