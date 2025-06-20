#include "../../include/Config.hpp"

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
	if (t["prefix"].type != toml::Table::NONE) l.prefix = t["prefix"].asStr();
	if (t["root"].type != toml::Table::NONE) l.root = t["root"].asStr();
	if (t["autoindex"].type != toml::Table::NONE) l.autoindex = t["autoindex"].asStr();
	if (t["upload"].type != toml::Table::NONE) l.upload = t["upload"].asStr();
	if (t["client_body"].type != toml::Table::NONE) l.client_body = t["client_body"].asStr();

	FillVector(t, "index", l.index);
	FillVector(t, "allowed_methods", l.allowed_methods);
	FillVector(t, "error_page", l.error_page);
	FillVector(t, "redirect", l.redirect);
	FillVector(t, "cgi", l.cgi);

	return l;
}

static ServerConfig FillServer(toml::Table& t) {
	ServerConfig s;
	if (t["host"].type != toml::Table::NONE) s.host = t["host"].asStr("127.0.0.1");
	if (t["root"].type != toml::Table::NONE) s.root = t["root"].asStr("./website/1");
	if (t["client_body"].type != toml::Table::NONE) s.client_body = t["client_body"].asStr();
	if (t["upload_path"].type != toml::Table::NONE) s.upload_path = t["upload_path"].asStr();

	FillVector(t, "port", s.port);
	FillVector(t, "redirect", s.redirect);
	FillVector(t, "server_name", s.server_name);
	FillVector(t, "allowed_methods", s.allowed_methods);
	FillVector(t, "index", s.index);
	FillVector(t, "error_page", s.error_page);

	toml::Table& locs = t["location"];
	FOR_EACH(std::vector<toml::Table>, locs.vec, it)
		s.locations.push_back(FillLocation(*it));

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
}

Config::~Config() {}

std::vector<ServerConfig>& Config::getServers() {
	return _servers;
}

void Config::Print() {
	FOR_EACH(std::vector<ServerConfig>, _servers, it) {
		it->Print();
	}

	// TODO: debug print
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
		case ERROR_INVALID_CLIENT_BODY:
			cout << "ERROR_INVALID_CLIENT_BODY" << endl;
			break;
		case ERROR_INVALID_LOCATION:
			cout << "ERROR_INVALID_LOCATION" << endl;
			break;
		case ERROR_INVALID_CGI:
			cout << "ERROR_INVALID_CGI" << endl;
			break;
		case ERROR_INVALID_AUTOINDEX:
			cout << "ERROR_INVALID_AUTOINDEX" << endl;
			break;
		case ERROR_INVALID_UPLOAD:
			cout << "ERROR_INVALID_UPLOAD" << endl;
			break;
		case ERROR_INVALID_REDIRECT:
			cout << "ERROR_INVALID_REDIRECT" << endl;
			break;
		case ERROR_INVALID_PREFIX:
			cout << "ERROR_INVALID_PREFIX" << endl;
			break;

	}
}

Config::e_error Config::PreValidate(toml::Table& config) {
	// TODO: debug print
	// 	std::cout << "server type: " << config["server"].getTypeName() << std::endl;
	// 	std::cout << "server size: " << config["server"].Size() << std::endl;

	if (config.getTypeName() != "TABLE")
		return ERROR_INVALID_SERVER;
	return ERROR_NONE;
}

Config::e_error Config::PostValidate() {
	if (_servers.empty())
		return ERROR_INVALID_SERVER;
	return ERROR_NONE;
}
