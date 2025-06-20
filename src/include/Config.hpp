#pragma once

#include "Utils.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include <cstdlib>
#include "../toml/Table.hpp"
#include "../toml/Toml.hpp"

using namespace std;

typedef vector<string> StrVec;

typedef struct s_LocationConfig {
	string prefix;
	string root;
	string autoindex;
	string upload;
	string client_body;
	vector<string> cgi;
	vector<string> error_page;
	vector<string> redirect;
	vector<string> index;
	vector<string> allowed_methods;

	void Print(int indent = 0) {
		string s(indent, ' ');
		if (!prefix.empty()) cout << s << "\tprefix: " << prefix << endl;
		if (!root.empty()) cout << s << "\troot: " << root << endl;
		if (!index.empty()) {
			cout << s << "\tindex: ";
			FOR_EACH(vector<string>, index, it) cout << *it << ", ";
			cout << endl;
		}
		if (!cgi.empty()) {
			cout << s << "\tcgi: ";
			FOR_EACH(vector<string>, cgi, it) cout << *it << ", ";
			cout << endl;
		}
		if (!autoindex.empty()) cout << s << "\tautoindex: " << autoindex << endl;
		if (!upload.empty()) cout << s << "\tupload: " << upload << endl;
		if (!client_body.empty()) cout << s << "\tclient_body: " << client_body << endl;
		if (!error_page.empty()) {
			cout << s << "\terror_page: ";
			FOR_EACH(vector<string>, error_page, it) cout << *it << " | ";
			cout << endl;
		}
		if (!redirect.empty()) {
			cout << s << "\tredirect: ";
			FOR_EACH(vector<string>, redirect, it) cout << *it << " | ";
			cout << endl;
		}
		if (!allowed_methods.empty()) {
			cout << s << "\tallowed_methods: ";
			FOR_EACH(vector<string>, allowed_methods, it) cout << *it << " ";
			cout << endl;
		}
	}
} LocationConfig;

typedef struct s_ServerConfig {
	vector<string> port;
	string host;
	string root;
	string client_body;
	string upload_path;
	vector<string> redirect;
	vector<string> server_name;
	vector<string> error_page;
	vector<string> allowed_methods;
	vector<string> index;
	vector<LocationConfig> locations;

	void Print() {
		cout << "==== ServerConfig ====\n";
		if (!port.empty()) {
			cout << "port: ";
			FOR_EACH(StrVec, port, it) cout << *it << " ";
			cout << endl;
		}
		if (!host.empty()) cout << "host: " << host << endl;
		if (!upload_path.empty()) cout << "upload_path: " << upload_path << endl;
		if (!server_name.empty()) {
			cout << "server_name: ";
			FOR_EACH(StrVec, server_name, it) cout << *it << " ";
			cout << endl;
		}
		if (!root.empty()) cout << "root: " << root << endl;
		if (!error_page.empty()) {
			cout << "error_page: ";
			FOR_EACH(StrVec, error_page, it) cout << *it << " | ";
			cout << endl;
		}
		if (!client_body.empty()) cout << "client_body: " << client_body << endl;
		if (!redirect.empty()) {
			cout << "redirect: ";
			FOR_EACH(StrVec, redirect, it) cout << *it << " | ";
			cout << endl;
		}
		if (!allowed_methods.empty()) {
			cout << "allowed_methods: ";
			FOR_EACH(StrVec, allowed_methods, it) cout << *it << " ";
			cout << endl;
		}
		if (!index.empty()) {
			cout << "index: ";
			FOR_EACH(StrVec, index, it) cout << *it << " ";
			cout << endl;
		}
		FOR_EACH(vector<LocationConfig>, locations, it) {
			cout << "\t==== LocationConfig ====\n";
			it->Print(2);
			cout << "\t==== END LocationConfig ====" << endl;
		}
		cout << "==== END ServerConfig ====\n";
	}
} ServerConfig;

class Config {
public:
	Config(toml::Table& config);
	~Config();
	enum e_error {
		ERROR_NONE,
		ERROR_UNKNOWN_KEY,
		ERROR_INVALID_SERVER,
		ERROR_INVALID_PORT,
		ERROR_INVALID_HOST,
		ERROR_INVALID_SERVER_NAME,
		ERROR_INVALID_INDEX,
		ERROR_INVALID_ALLOWED_METHODS,
		ERROR_INVALID_ROOT,
		ERROR_INVALID_ERROR_PAGE,
		ERROR_INVALID_CLIENT_BODY,
		ERROR_INVALID_LOCATION,
		ERROR_INVALID_PREFIX,
		ERROR_INVALID_REDIRECT,
		ERROR_INVALID_CGI,
		ERROR_INVALID_AUTOINDEX,
		ERROR_INVALID_UPLOAD
	};
	e_error error;
	vector<ServerConfig>& getServers() ;
	void Print();

private:
	enum e_state {
		OK,
		INVALID_KEY,
		INVALID_VALUE
	};
	vector<ServerConfig> _servers;
	e_error PreValidate(toml::Table &config);
	e_error PostValidate();
};
