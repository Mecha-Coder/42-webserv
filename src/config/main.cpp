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
	config.Print();  // Step 1: Print config to verify correctness

	const std::vector<ServerConfig>& servers = config.getServers();

	std::cout << "\n=== TESTING CONFIG ACCESS ===\n";

	// Step 2: Print all server names
	std::cout << "Server Names:\n";
	for (size_t i = 0; i < servers.size(); i++) {
		for (size_t j = 0; j < servers[i].server_name.size(); j++) {
			std::cout << "- " << servers[i].server_name[j] << "\n";
		}
	}

	// Step 3: Access a specific field by key (first server)
	if (!servers.empty()) {
		const ServerConfig& s = servers[0];
		std::cout << "\nFirst server's root: " << s.root << "\n";
		std::cout << "First server's ports:\n";
		for (size_t j = 0; j < s.port.size(); j++)
			std::cout << "- " << s.port[j] << "\n";

		// Step 4: List all route paths
		std::cout << "\nRoutes:\n";
		for (size_t k = 0; k < s.locations.size(); k++) {
			std::cout << "- " << s.locations[k].prefix << "\n";
		}

		// Step 5: Show CGI extensions per route (if any)
		std::cout << "\nCGI for each route:\n";
		for (size_t k = 0; k < s.locations.size(); k++) {
			if (!s.locations[k].cgi.empty()) {
				std::cout << "  Route " << s.locations[k].prefix << " uses CGI: ";
				for (size_t c = 0; c < s.locations[k].cgi.size(); c++)
					std::cout << s.locations[k].cgi[c] << " ";
				std::cout << "\n";
			}
		}
	}

	std::cout << "=== END CONFIG TEST ===\n";

	delete table;
	return 0;
}
