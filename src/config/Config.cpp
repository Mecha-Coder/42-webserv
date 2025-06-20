#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include "../toml/Table.hpp"
#include "../toml/Parser.hpp"
#include "Config.hpp"

static bool isType(toml::Table &table, const std::string &key, toml::Table::EToml type) {
	return table[key].type == toml::Table::NONE || table[key].type == type;
}

static bool isStrArray(toml::Table &table, const std::string &key) {
	toml::Table &t = table[key];
	if (t.type == toml::Table::NONE)
		return true;
	if (t.type != toml::Table::ARRAY)
		return false;
	for (size_t i = 0; i < t.vec.size(); ++i) {
		if (t.vec[i].type != toml::Table::STRING)
			return false;
	}
	return true;
}

static Config::e_error ValidateLocation(toml::Table &location) {
	const std::string keys[] = {"prefix", "autoindex", "upload", "redirect", "index", "allowed_methods", "cgi"};
	FOR_EACH(toml::Table::TomlMap, location.map, it) {
		if (it->first == "location") continue;
		if (std::find(keys, keys + sizeof(keys)/sizeof(*keys), it->first) == keys + sizeof(keys)/sizeof(*keys)) {
			std::cerr << "Unknown key '" << it->first << "' at line " << it->second->line << std::endl;
			return Config::ERROR_UNKNOWN_KEY;
		}
	}
	if (!isType(location, "prefix", toml::Table::STRING)) {
		std::cerr << "Invalid type for 'prefix' at line " << location["prefix"].line << std::endl;
		return Config::ERROR_INVALID_PREFIX;
	}
	if (!isType(location, "autoindex", toml::Table::STRING)) {
		std::cerr << "Invalid type for 'autoindex' at line " << location["autoindex"].line << std::endl;
		return Config::ERROR_INVALID_AUTOINDEX;
	}
	if (!isType(location, "upload", toml::Table::STRING)) {
		std::cerr << "Invalid type for 'upload' at line " << location["upload"].line << std::endl;
		return Config::ERROR_INVALID_UPLOAD;
	}
	if (!isType(location, "redirect", toml::Table::STRING)) {
		std::cerr << "Invalid type for 'redirect' at line " << location["redirect"].line << std::endl;
		return Config::ERROR_INVALID_REDIRECT;
	}
	// if (!isStrArray(location, "redirect")) {
	// 	int line = -1;
	// 	if (location["redirect"].type == toml::Table::ARRAY && !location["redirect"].vec.empty())
	// 		line = location["redirect"].vec[0].line;
	// 	std::cerr << "Invalid type for 'redirect' at line " << line << std::endl;
	// 	return Config::ERROR_INVALID_REDIRECT;
	// }
	if (!isStrArray(location, "index")) {
		int line = -1;
		if (location["index"].type == toml::Table::ARRAY && !location["index"].vec.empty())
			line = location["index"].vec[0].line;
		std::cerr << "Invalid type for 'index' at line " << line << std::endl;
		return Config::ERROR_INVALID_INDEX;
	}
	if (!isStrArray(location, "allowed_methods")) {
		int line = -1;
		if (location["allowed_methods"].type == toml::Table::ARRAY && !location["allowed_methods"].vec.empty())
			line = location["allowed_methods"].vec[0].line;
		std::cerr << "Invalid type for 'allowed_methods' at line " << line << std::endl;
		return Config::ERROR_INVALID_ALLOWED_METHODS;
	}
	if (!isStrArray(location, "cgi")) {
		int line = -1;
		if (location["cgi"].type == toml::Table::ARRAY && !location["cgi"].vec.empty())
			line = location["cgi"].vec[0].line;
		std::cerr << "Invalid type for 'cgi' at line " << line << std::endl;
		return Config::ERROR_INVALID_CGI;
	}
	return Config::ERROR_NONE;
}

static Config::e_error ValidateServer(toml::Table &server) {
	const std::string keys[] = {"host", "server_name", "port", "root", "upload_path", "client_body", "error_page", "location"};
	FOR_EACH(toml::Table::TomlMap, server.map, it) {
		if (it->first == "server") continue;
		if (std::find(keys, keys + sizeof(keys)/sizeof(*keys), it->first) == keys + sizeof(keys)/sizeof(*keys)) {
			std::cerr << "Unknown key '" << it->first << "' at line " << it->second->line << std::endl;
			return Config::ERROR_UNKNOWN_KEY;
		}
	}
	if (!isType(server, "host", toml::Table::STRING)) {
		std::cerr << "Invalid type for 'host' at line " << server["host"].line << std::endl;
		return Config::ERROR_INVALID_HOST;
	}
	if (!isStrArray(server, "server_name")) {
		int line = -1;
		if (server["server_name"].type == toml::Table::ARRAY && !server["server_name"].vec.empty())
			line = server["server_name"].vec[0].line;
		std::cerr << "Invalid type for 'server_name' at line " << line << std::endl;
		return Config::ERROR_INVALID_SERVER_NAME;
	}
	if (!isStrArray(server, "port")) {
		int line = -1;
		if (server["port"].type == toml::Table::ARRAY && !server["port"].vec.empty())
			line = server["port"].vec[0].line;
		std::cerr << "Invalid type for 'port' at line " << line << std::endl;
		return Config::ERROR_INVALID_PORT;
	}
	if (!isType(server, "root", toml::Table::STRING)) {
		std::cerr << "Invalid type for 'root' at line " << server["root"].line << std::endl;
		return Config::ERROR_INVALID_ROOT;
	}
	if (!isType(server, "client_body", toml::Table::STRING)) {
		std::cerr << "Invalid type for 'client_body' at line " << server["client_body"].line << std::endl;
		return Config::ERROR_INVALID_CLIENT_BODY;
	}
	if (!isStrArray(server, "error_page")) {
		int line = -1;
		if (server["error_page"].type == toml::Table::ARRAY && !server["error_page"].vec.empty())
			line = server["error_page"].vec[0].line;
		std::cerr << "Invalid type for 'error_page' at line " << line << std::endl;
		return Config::ERROR_INVALID_ERROR_PAGE;
	}
	if (server["location"].type == toml::Table::ARRAY) {
		for (size_t i = 0; i < server["location"].vec.size(); ++i) {
			Config::e_error err = ValidateLocation(server["location"].vec[i]);
			if (err != Config::ERROR_NONE) {
				std::cerr << "Invalid location block at line " << server["location"].vec[i].line << std::endl;
				return err;
			}
		}
	}
	return Config::ERROR_NONE;
}

static void FillVector(toml::Table& t, const std::string& key, std::vector<std::string>& v, const std::string& fallback = "") {
	toml::Table& node = t[key];
	if (node.type == toml::Table::NONE)
		return;
	if (node.type == toml::Table::ARRAY) {
		FOR_EACH(std::vector<toml::Table>, node.vec, it)
			v.push_back(it->asStr(fallback));
	} else {
		v.push_back(node.asStr(fallback));
	}
}

static LocationConfig FillLocation(toml::Table& t) {
	LocationConfig l;
	if (t["prefix"].type == toml::Table::STRING)
		l.prefix = t["prefix"].asStr();
	if (t["autoindex"].type == toml::Table::STRING)
		l.autoindex = t["autoindex"].asStr();
	if (t["upload"].type == toml::Table::STRING)
		l.upload = t["upload"].asStr();
	if (t["redirect"].type == toml::Table::STRING) 
		l.redirect = t["redirect"].asStr();

	FillVector(t, "index", l.index);
	FillVector(t, "allowed_methods", l.allowed_methods);
	// FillVector(t, "redirect", l.redirect);
	FillVector(t, "cgi", l.cgi);

	return l;
}

static ServerConfig FillServer(toml::Table& t) {
	ServerConfig s;
	if (t["host"].type == toml::Table::STRING)
		s.host = t["host"].asStr("127.0.0.1");
	if (t["root"].type == toml::Table::STRING)
		s.root = t["root"].asStr("./website/1");
	if (t["client_body"].type == toml::Table::STRING)
		s.client_body = t["client_body"].asStr();

	FillVector(t, "port", s.port);
	FillVector(t, "server_name", s.server_name);
	FillVector(t, "error_page", s.error_page);

	if (t["location"].type == toml::Table::ARRAY) {
		FOR_EACH(std::vector<toml::Table>, t["location"].vec, it) {
			s.locations.push_back(FillLocation(*it));
		}
	}

	return s;
}

Config::Config(toml::Table& config) : error(ERROR_NONE) {
	error = PreValidate(config);
	if (error != ERROR_NONE)
		return;

	toml::Table& servers = config["server"];
	if (servers.type != toml::Table::ARRAY || servers.vec.empty()) {
		error = ERROR_INVALID_SERVER;
		return;
	}

	FOR_EACH(std::vector<toml::Table>, servers.vec, it) {
		_servers.push_back(FillServer(*it));
	}

	error = PostValidate();
	if (error != ERROR_NONE) {
		_servers.clear();
		std::cerr << "Post-validation failed with error code: " << error << std::endl;
		return;
	}
}

Config::~Config() {}

const std::vector<ServerConfig>& Config::getServers() const {
	return _servers;
}

void Config::Print() {
	FOR_EACH(std::vector<ServerConfig>, _servers, it) {
		it->Print();
	}

	// TODO: debug print
	switch (error) {
		case ERROR_NONE: cout << "ERROR_NONE" << endl; break;
		case ERROR_UNKNOWN_KEY: cout << "ERROR_UNKNOWN_KEY" << endl; break;
		case ERROR_INVALID_SERVER: cout << "ERROR_INVALID_SERVER" << endl; break;
		case ERROR_INVALID_PORT: cout << "ERROR_INVALID_PORT" << endl; break;
		case ERROR_INVALID_HOST: cout << "ERROR_INVALID_HOST" << endl; break;
		case ERROR_INVALID_SERVER_NAME: cout << "ERROR_INVALID_SERVER_NAME" << endl; break;
		case ERROR_INVALID_INDEX: cout << "ERROR_INVALID_INDEX" << endl; break;
		case ERROR_INVALID_ALLOWED_METHODS: cout << "ERROR_INVALID_ALLOWED_METHODS" << endl; break;
		case ERROR_INVALID_ROOT: cout << "ERROR_INVALID_ROOT" << endl; break;
		case ERROR_INVALID_ERROR_PAGE: cout << "ERROR_INVALID_ERROR_PAGE" << endl; break;
		case ERROR_INVALID_CLIENT_BODY: cout << "ERROR_INVALID_CLIENT_BODY" << endl; break;
		case ERROR_INVALID_LOCATION: cout << "ERROR_INVALID_LOCATION" << endl; break;
		case ERROR_INVALID_PREFIX: cout << "ERROR_INVALID_PREFIX" << endl; break;
		case ERROR_INVALID_REDIRECT: cout << "ERROR_INVALID_REDIRECT" << endl; break;
		case ERROR_INVALID_CGI: cout << "ERROR_INVALID_CGI" << endl; break;
		case ERROR_INVALID_AUTOINDEX: cout << "ERROR_INVALID_AUTOINDEX" << endl; break;
		case ERROR_INVALID_UPLOAD: cout << "ERROR_INVALID_UPLOAD" << endl; break;
	}
}

Config::e_error Config::PreValidate(toml::Table& config) {
	// TODO: debug print
	// 	std::cout << "server type: " << config["server"].getTypeName() << std::endl;
	// 	std::cout << "server size: " << config["server"].Size() << std::endl;

	if (config.getTypeName() != "TABLE")
		return ERROR_INVALID_SERVER;
	if (config.map.empty()) {
		std::cerr << "Empty config file detected\n";
		return ERROR_INVALID_SERVER;
	}
	if (config["server"].type != toml::Table::ARRAY || config["server"].vec.empty()) {
		std::cerr << "Missing or empty 'server' block in config\n";
		return ERROR_INVALID_SERVER;
	}
	toml::Table& t = config["server"];
	for (size_t i = 0; i < t.vec.size(); i++) {
		if ((error = ValidateServer(t[i])) != Config::ERROR_NONE)
			return error;
	}
	return ERROR_NONE;
}

Config::e_error Config::PostValidate() {
	if (_servers.empty())
		return Config::ERROR_INVALID_SERVER;
	for (size_t i = 0; i < _servers.size(); ++i) {
		const ServerConfig& server = _servers[i];
		for (size_t j = 0; j < server.port.size(); ++j) {
			const std::string& portStr = server.port[j];
			char* end;
			long port = std::strtol(portStr.c_str(), &end, 10);
			if (*end != '\0' || port < 1 || port > 65535)
				return Config::ERROR_INVALID_PORT;
		}
		for (size_t j = 0; j < server.error_page.size(); ++j) {
			const std::string& entry = server.error_page[j];
			size_t eq = entry.find('=');
			if (eq == std::string::npos || eq == 0 || eq == entry.length() - 1)
				return Config::ERROR_INVALID_ERROR_PAGE;
			std::string codeStr = entry.substr(0, eq);
			char* end;
			int code = std::strtol(codeStr.c_str(), &end, 10);
			if (*end != '\0' || code < 100 || code > 599)
				return Config::ERROR_INVALID_ERROR_PAGE;
		}
		if (!server.root.empty() && !(server.root[0] == '.' || server.root[0] == '/'))
			return Config::ERROR_INVALID_ROOT;
		if (!server.client_body.empty()) {
			const std::string& body = server.client_body;
			size_t i = 0;
			while (i < body.length() && isdigit(body[i])) ++i;
			if (i == 0 || (i < body.length() && std::string("kKmMgG").find(body[i]) == std::string::npos))
				return Config::ERROR_INVALID_CLIENT_BODY;
		}
		for (size_t j = 0; j < server.locations.size(); ++j) {
			const LocationConfig& loc = server.locations[j];
			if (loc.prefix.empty() || loc.prefix[0] != '/')
				return Config::ERROR_INVALID_PREFIX;
		}
	}
	return Config::ERROR_NONE;
}
