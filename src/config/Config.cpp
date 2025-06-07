#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include "../toml/Table.hpp"
#include "Config.hpp"

void FillArr(toml::Table& t, string key, vector<string>& v, ServerConfig& s) {
    if (t[key].isType(toml::Table::NONE))
    {
        v = s.allowed_methods;
        return;
    }
    FOR_EACH(vector<toml::Table>, t[key].vec, it) {
        v.push_back(it->asStr(""));
    }
}

LocationConfig FillLocation(toml::Table& location, ServerConfig& s) {
	(void)location;
	LocationConfig l;

	l.prefix = location["prefix"].asStr("/");
	l.root = location["root"].asStr("s.root");

    FillArr(location, "index", l.index, s);
    FillArr(location, "allowed_methods", l.allowed_methods, s);
    FillArr(location, "error_page", l.error_page, s);
    FillArr(location, "redirect", l.redirect, s);
    FillArr(location, "cgi_path", l.cgi_path, s);

	l.autoindex = location["autoindex"].asStr("off");
	l.upload_path = location["upload_path"].asStr(s.upload_path);
	l.client_max_body_size = location["_clientMaxBodySize"].asStr(s.client_max_body_size);
	return l;
}

ServerConfig FillServer(toml::Table& server) {
	ServerConfig s;
	s.port = server["port"].asInt(s.port);
	s.host = server["host"].asStr("127.0.0.1");
	s.upload_path = server["upload_path"].asStr("www/upload");
	s.root = server["root"].asStr("www");
	s.client_max_body_size = server["_clientMaxBodySize"].asStr("1m");

    FOR_EACH(vector<toml::Table>, server["index"].vec, it) {
        s.index.push_back(it->asStr("default.com"));
    }

	FOR_EACH(vector<toml::Table>, server["redirect"].vec, it) {
		s.redirect.push_back(it->asStr(""));
	}

	FOR_EACH(vector<toml::Table>, server["server_name"].vec, it) {
		s.server_name.push_back(it->asStr());
	}
    FOR_EACH(vector<toml::Table>, server["allowed_methods"].vec, it) {
        s.allowed_methods.push_back(it->asStr(""));
    }
    FOR_EACH(vector<toml::Table>, server["error_page"].vec, it) {
        s.error_page.push_back(it->asStr(""));
    }

	toml::Table& t = server["location"];
	for (size_t i = 0; i < t.vec.size(); i++) {
		s.locations.push_back(FillLocation(t[i], s));
	}
	return s;
}

bool optional_is(toml::Table &Table, const string &key, toml::Table::EToml type) {
	return Table[key].isType(toml::Table::NONE) || Table[key].isType(type);
}

bool optional_str_arr(toml::Table &Table, const string &key)
{
	toml::Table& t = Table[key];
	if (!optional_is(Table, key, toml::Table::ARRAY))
		return false;
	for (size_t i = 0; i < t.vec.size(); i++) {
		if (!t[i].isType(toml::Table::STRING))
			return false;
	}
	return true;
}

Config::e_error validate_location(toml::Table &location)
{
	string location_keys[] = {"prefix", "root", "upload_path", "autoindex", "redirect", "allowed_methods", "index", "cgi_path", "error_page", "_clientMaxBodySize"};
	FOR_EACH(toml::Table::TomlMap, location.map, it) {
		if (find(begin(location_keys), end(location_keys), it->first) == end(location_keys))
			return Config::ERROR_UNKNOWN_KEY;
	}

	if (!optional_is(location, "prefix", toml::Table::STRING))
		return Config::ERROR_INVALID_PORT;
	else if (!optional_is(location, "root", toml::Table::STRING))
		return Config::ERROR_INVALID_ROOT;
	else if (!optional_is(location, "upload_path", toml::Table::STRING))
		return Config::ERROR_INVALID_UPLOAD_PATH;
	else if (!optional_is(location, "autoindex", toml::Table::STRING))
		return Config::ERROR_INVALID_AUTOINDEX;
	else if (!optional_is(location, "client_max_body_size", toml::Table::STRING))
		return Config::ERROR_INVALID_CLIENT_MAX_BODY_SIZE;
	else if (!optional_str_arr(location, "redirect"))
		return Config::ERROR_INVALID_REDIRECT;
	else if (!optional_str_arr(location, "cgi_path"))
		return Config::ERROR_INVALID_CGI_PATH;
	else if (!optional_str_arr(location, "index"))
		return Config::ERROR_INVALID_INDEX;
	else if (!optional_str_arr(location, "allowed_methods"))
		return Config::ERROR_INVALID_ALLOWED_METHODS;
	else if (!optional_str_arr(location, "error_page"))
		return Config::ERROR_INVALID_ERROR_PAGE;
	return Config::ERROR_NONE;
}

Config::e_error validate_location_list(toml::Table &location) {
	Config::e_error error;
	if (!location.isType(toml::Table::ARRAY))
		return Config::ERROR_INVALID_LOCATION;
	for (size_t i = 0; i < location.vec.size(); i++) {
		error = validate_location(location[i]);
		if (error != Config::ERROR_NONE)
			return error;
	}
	return Config::ERROR_NONE;
}

Config::e_error ValidServer(toml::Table& server) {
	Config::e_error error;
	string server_keys[] = {"port", "host", "index", "server_name", "allowed_methods", "root", "error_page", "_clientMaxBodySize", "location", "upload_path", "redirect"};
	FOR_EACH(toml::Table::TomlMap, server.map, it) {
		if (find(begin(server_keys), end(server_keys), it->first) == end(server_keys))
			return Config::ERROR_UNKNOWN_KEY;
	}

	if (!optional_is(server, "port", toml::Table::STRING))
		return Config::ERROR_INVALID_PORT;
	else if (!optional_is(server, "upload_path", toml::Table::STRING))
		return Config::ERROR_INVALID_PORT;
	else if (!optional_is(server, "host", toml::Table::STRING))
		return Config::ERROR_INVALID_HOST;
	else if (!optional_is(server, "_clientMaxBodySize", toml::Table::STRING))
		return Config::ERROR_INVALID_CLIENT_MAX_BODY_SIZE;
	else if (!optional_str_arr(server, "redirect"))
		return Config::ERROR_INVALID_REDIRECT;
	else if (!optional_str_arr(server, "index"))
		return Config::ERROR_INVALID_INDEX;
	else if (!optional_str_arr(server, "server_name"))
		return Config::ERROR_INVALID_SERVER_NAME;
	else if (!optional_str_arr(server, "allowed_methods"))
		return Config::ERROR_INVALID_ALLOWED_METHODS;
	else if (!optional_is(server, "root", toml::Table::STRING))
		return Config::ERROR_INVALID_ROOT;
	else if (!optional_str_arr(server, "error_page"))
		return Config::ERROR_INVALID_ERROR_PAGE;
	else if ((error = validate_location_list(server["location"])) != Config::ERROR_NONE)
			return error;
	return Config::ERROR_NONE;
}

Config::e_error Config::PreValidate(toml::Table& config) {
	
	toml::Table& t = config["server"];
	if (t.isType(toml::Table::NONE) || !t.isType(toml::Table::ARRAY))
		return Config::ERROR_INVALID_SERVER;

	for (size_t i = 0; i < t.vec.size(); i++) {
		if ((error = ValidServer(t[i])) != Config::ERROR_NONE)
			return error;
	}

	return Config::ERROR_NONE;
}

Config::e_error Config::PostValidate() {
	if (_servers.empty())
		return Config::ERROR_INVALID_SERVER;
	for (size_t i = 0; i < _servers.size(); i++) {
		if (_servers[i].port < 0 || _servers[i].port > 65535 )
			return Config::ERROR_INVALID_PORT;
		if (_servers[i].error_page.size() < 2 && !_servers[i].error_page.empty())
			return Config::ERROR_INVALID_ERROR_PAGE;
		for (size_t j = 0; j < _servers[i].error_page.size() - 1; j++) {
			int code = atoi(_servers[i].error_page[j].c_str());
			if (code < 100 || code > 599)
				return Config::ERROR_INVALID_ERROR_PAGE;
		}
	}
	return Config::ERROR_NONE;
}

Config::Config(toml::Table& config) {
	if ((error = PreValidate(config) )!= Config::ERROR_NONE)
	{
		Print();
		cout << "ERROR: Invalid configuration file" << endl;
		exit(1);
	}
	toml::Table& t = config["server"];
	for (size_t i = 0; i < t.vec.size(); i++) {
		_servers.push_back(FillServer(t[i]));
	}
	error = PostValidate();
	if (error != Config::ERROR_NONE) {
		Print();
		cout << "ERROR: Invalid configuration file" << endl;
		exit(1);
	}
}

Config::~Config() {

}

const vector<ServerConfig> &Config::getServers() const {
    return _servers;
}

void Config::Print() {
	cout << "\n<==================== Config ====================>" << endl;

	for (size_t i = 0; i < _servers.size(); i++)
		_servers[i].Print();
	
	switch (error) {
		case ERROR_NONE:
			cout << "ERROR_NONE" << endl;
			break;
		case ERROR_INVALID_SERVER:
			cout << "ERROR_INVALID_SERVER" << endl;
			break;
		case ERROR_INVALID_PORT:
			cout << "ERROR_INVALID_PORT" << endl;
			break;
		case ERROR_INVALID_HOST:
			cout << "ERROR_INVALID_HOST" << endl;
			break;
		case ERROR_UNKNOWN_KEY:
			cout << "ERROR_UNKNOWN" << endl;
			break;
		case ERROR_INVALID_INDEX:
			cout << "ERROR_INVALID_INDEX" << endl;
			break;
		case ERROR_INVALID_SERVER_NAME:
			cout << "ERROR_INVALID_SERVER_NAME" << endl;
			break;
		case ERROR_INVALID_ALLOWED_METHODS:
			cout << "ERROR_INVALID_ALLOWED_METHODS" << endl;
			break;

		case ERROR_INVALID_ROOT:
			cout << "ERROR_INVALID_ROOT" << endl;
			break;
		case ERROR_INVALID_ERROR_PAGE:
			cout << "ERROR_INVALID_ERROR_PAGE" << endl;
			break;
		case ERROR_INVALID_CLIENT_MAX_BODY_SIZE:
			cout << "ERROR_INVALID_CLIENT_MAX_BODY_SIZE" << endl;
			break;
		case ERROR_INVALID_LOCATION:
			cout << "ERROR_INVALID_LOCATION" << endl;
			break;
		case ERROR_INVALID_CGI_PATH:
			cout << "ERROR_INVALID_CGI_PATH" << endl;
			break;
		case ERROR_INVALID_AUTOINDEX:
			cout << "ERROR_INVALID_AUTOINDEX" << endl;
			break;
		case ERROR_INVALID_UPLOAD_PATH:
			cout << "ERROR_INVALID_UPLOAD_PATH" << endl;
			break;
		case ERROR_INVALID_REDIRECT:
			cout << "ERROR_INVALID_REDIRECT" << endl;
			break;
		case ERROR_INVALID_PREFIX:
			cout << "ERROR_INVALID_PREFIX" << endl;
			break;

	}

	cout << "\n<==================== END Config ====================>\n" << endl;
}
