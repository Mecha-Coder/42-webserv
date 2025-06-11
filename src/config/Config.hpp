#pragma once

#include "../toml/Toml.hpp"
#include "../utils/Utils.hpp"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

typedef vector<string> StrVec;
struct LocationConfig {
	string prefix;
	string root;
	string autoindex;
	string upload_path;
	vector<string> cgi_path;
	string client_max_body_size;
	vector<string> error_page;
	vector<string> redirect;
	vector<string> index;
    vector<string> allowed_methods;

	void Print(int indent = 0) {
		string s(indent, ' ');
		cout << s << "\t\tprefix: " << prefix << endl;
		cout << s << "\t\troot: " << root << endl;
		cout << s << "\t\tindex: ";
		FOR_EACH(vector<string>, index, it) {
			cout << "\t\t" << *it << ", ";
		}
		cout << endl;
		cout << s << "\t\tcgi_path: ";
		FOR_EACH(vector<string>, cgi_path, it) {
			cout << "\t\t" << *it << ", ";
		}
		cout << endl;
		cout << s << "\t\tautoindex: " << autoindex << endl;
		cout << s << "\t\tupload_path: " << upload_path << endl;
		cout << s << "\t\tclient_max_body_size: " << client_max_body_size << endl;
	}
};

struct ServerConfig {

	unsigned short port;
	string host;
	string root;
	string client_max_body_size;
	string upload_path;
	vector<string> redirect;
	vector<string> server_name;
	vector<string> error_page;
    vector<string> allowed_methods;
    vector<string> index;

	vector<LocationConfig> locations;

	void Print() {

		cout << "\t==== ServerConfig ====" << endl;
		cout << "\tlisten: " << port << endl;
		cout << "\thost: " << host << endl;
		cout << "\tupload_path: " << upload_path << endl;
		cout << "\tserver_name: ";
		FOR_EACH(StrVec, server_name, it) {
			cout << "\t" << *it << " ";
		}
		cout << endl;
		cout << "\troot: " << root << endl;
		cout << "\terror_page: ";
		for (size_t i = 0; i < error_page.size(); i++) {
			cout << error_page[i] << " | " ;
		}
		cout << endl;
		cout << "\tclient_max_body_size: " << client_max_body_size << endl;
		for (size_t i = 0; i < locations.size(); i++) {
			cout << "\t\t" << "==== LocationConfig ====" << endl;
			cout << "\t\tlocation " << i << ":" << endl;
			locations[i].Print(1);
			cout << "\t\t" << "==== END LocationConfig ====\n" << endl;
		}
		cout << "\t==== END ServerConfig ====" << endl;
	}
};

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
		ERROR_INVALID_CLIENT_MAX_BODY_SIZE,
		ERROR_INVALID_LOCATION,
		ERROR_INVALID_PREFIX,
		ERROR_INVALID_REDIRECT,
		ERROR_INVALID_CGI_PATH,
		ERROR_INVALID_AUTOINDEX,
		ERROR_INVALID_UPLOAD_PATH
	};
	e_error error;
    const vector<ServerConfig>& getServers() const;

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
