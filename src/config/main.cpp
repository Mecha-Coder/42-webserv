#include <iostream>
#include "../toml/Toml.hpp"
#include "Config.hpp"

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <config.toml>\n";
		return 1;
	}

	std::string filename = argv[1];
	toml::Table* table = toml::ParseFile(filename);

	if (!table) {
		std::cerr << "Failed to parse the TOML file.\n";
		return 1;
	}

	Config config(*table);
	config.Print();  // Print config output to stdout

	delete table;
	return 0;
}
