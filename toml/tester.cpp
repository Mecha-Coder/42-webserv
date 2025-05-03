#include "Table.hpp"
#include <iostream>
#include <string>
#include <vector>

// int main() {
// 	using namespace toml;

// 	Table servers(Table::ARRAY);

// 	Table s1(Table::TABLE);
// 	s1.Insert("server", new Table(std::string("127.0.0.1")));
// 	s1.Insert("port", new Table(std::string("8080")));
// 	servers.Push(s1);

// 	Table s2(Table::TABLE);
// 	s2.Insert("server", new Table(std::string("192.168.1.10")));
// 	s2.Insert("port", new Table(std::string("3000")));
// 	servers.Push(s2);

// 	Table s3(Table::TABLE);
// 	s3.Insert("server", new Table(std::string("10.0.0.5")));
// 	s3.Insert("port", new Table(std::string("9090")));
// 	servers.Push(s3);

// 	std::cout << "Server list:\n";

// 	const std::vector<Table>& entries = servers.GetVector();
// 	for (std::size_t i = 0; i < entries.size(); ++i) {
// 		const Table& entry = entries[i];
// 		std::cout << "Entry " << i << ":\n";
// 		std::cout << "  server: " << entry["server"].asStr("unknown") << "\n";
// 		std::cout << "  port: " << entry["port"].asStr("0") << "\n";

// 		std::cout << "  All pairs:\n";
// 		const Table::TomlMap& m = entry.GetMap();
// 		for (Table::TomlMap::const_iterator it = m.begin(); it != m.end(); ++it) {
// 			std::cout << "	" << it->first << ": " << it->second->asStr() << "\n";
// 		}
// 		std::cout << std::endl;
// 	}

// 	return 0;
// }

int main() {
	using namespace toml;

	// Create a Table representing a list of server configs
	Table servers(Table::ARRAY);

	// Define a few server entries
	Table s1(Table::TABLE);
	s1.Insert("server", new Table(std::string("127.0.0.1")));
	s1.Insert("port", new Table(std::string("8080")));

	Table s2(Table::TABLE);
	s2.Insert("server", new Table(std::string("192.168.1.10")));
	s2.Insert("port", new Table(std::string("3000")));

	Table s3(Table::TABLE);
	s3.Insert("server", new Table(std::string("10.0.0.5")));
	s3.Insert("port", new Table(std::string("9090")));
	s3.Insert("ssl", new Table(true));

	// Push entries to the array
	servers.Push(s1);
	servers.Push(s2);
	servers.Push(s3);

	// Access array using GetVector()
	const std::vector<Table>& entries = servers.GetVector();

	// Iterate and display entries using operator[]
	std::cout << "== Server Configurations ==\n";
	for (std::size_t i = 0; i < entries.size(); ++i) {
		const Table& entry = entries[i];
		std::cout << "Entry " << i << ":\n";

		// Basic access
		std::cout << "  server: " << entry["server"].asStr("unknown") << "\n";
		std::cout << "  port  : " << entry["port"].asStr("0") << "\n";

		// Optional key
		std::string sslStr = entry["ssl"].IsValid() ? (entry["ssl"].asBool() ? "enabled" : "disabled") : "not set";
		std::cout << "  ssl   : " << sslStr << "\n";

		// Iterate all key-value pairs using GetMap()
		const Table::TomlMap& m = entry.GetMap();
		std::cout << "  All fields:\n";
		for (Table::TomlMap::const_iterator it = m.begin(); it != m.end(); ++it) {
			std::cout << "    " << it->first << " = " << it->second->asStr() << " (type: "
			          << it->second->GetTypeName() << ")\n";
		}
		std::cout << "\n";
	}

	// Demonstrate Remove()
	std::cout << "== Demonstrating Remove() ==\n";
	Table modEntry = entries[0];
	std::cout << "Before removing 'port':\n";
	std::cout << "  port = " << modEntry["port"].asStr("N/A") << "\n";
	modEntry.Remove("port");
	std::cout << "After removing 'port':\n";
	std::cout << "  port = " << modEntry["port"].asStr("N/A") << "\n\n";

	// Demonstrate GetKeys() on a server entry
	std::cout << "== Demonstrating GetKeys() ==\n";
	std::vector<std::string> keys = s2.GetKeys();
	std::cout << "Keys in entry 1:\n";
	for (std::size_t i = 0; i < keys.size(); ++i) {
		std::cout << "  - " << keys[i] << "\n";
	}
	std::cout << "\n";

	// Demonstrate utility functions
	std::cout << "== Type and Utility Checks ==\n";
	std::cout << "servers type   : " << servers.GetTypeName() << "\n";
	std::cout << "entries size   : " << servers.Size() << "\n";
	std::cout << "first entry is table: " << (entries[0].isType(Table::TABLE) ? "yes" : "no") << "\n";
	std::cout << "ssl in entry 0 valid? " << (entries[0]["ssl"].IsValid() ? "yes" : "no") << "\n";

	return 0;
}
