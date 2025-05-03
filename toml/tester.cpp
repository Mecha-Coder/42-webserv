#include "Table.hpp"
#include <iostream>
#include <string>

int main() {
	using namespace toml;

	Table servers(Table::ARRAY);

	Table s1(Table::TABLE);
	s1.Insert("server", new Table(std::string("127.0.0.1")));
	s1.Insert("port", new Table(std::string("8080")));
	servers.Push(s1);

	Table s2(Table::TABLE);
	s2.Insert("server", new Table(std::string("192.168.1.10")));
	s2.Insert("port", new Table(std::string("3000")));
	servers.Push(s2);

	Table s3(Table::TABLE);
	s3.Insert("server", new Table(std::string("10.0.0.5")));
	s3.Insert("port", new Table(std::string("9090")));
	servers.Push(s3);

	std::cout << "Server list:\n";

	const std::vector<Table>& entries = servers.GetVector();
	for (std::size_t i = 0; i < entries.size(); ++i) {
		const Table& entry = entries[i];
		std::cout << "Entry " << i << ":\n";
		std::cout << "  server: " << entry["server"].asStr("unknown") << "\n";
		std::cout << "  port: " << entry["port"].asStr("0") << "\n";

		std::cout << "  All pairs:\n";
		const Table::TomlMap& m = entry.GetMap();
		for (Table::TomlMap::const_iterator it = m.begin(); it != m.end(); ++it) {
			std::cout << "    " << it->first << ": " << it->second->asStr() << "\n";
		}
		std::cout << std::endl;
	}

	return 0;
}
