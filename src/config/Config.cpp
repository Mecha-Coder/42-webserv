#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include "Table.hpp"
#include "Config.hpp"

using std::string;
using std::vector;
using std::map;
using std::cout;
using std::endl;

namespace {

const char* get_error_message(Config::e_error error) {
    static const map<Config::e_error, const char*> messages = {
        {Config::ERROR_NONE, "ERROR_NONE"},
        {Config::ERROR_INVALID_SERVER, "ERROR_INVALID_SERVER"},
        {Config::ERROR_INVALID_PORT, "ERROR_INVALID_PORT"},
        {Config::ERROR_INVALID_HOST, "ERROR_INVALID_HOST"},
        {Config::ERROR_UNKNOWN_KEY, "ERROR_UNKNOWN_KEY"},
        {Config::ERROR_INVALID_INDEX, "ERROR_INVALID_INDEX"},
        {Config::ERROR_INVALID_SERVER_NAME, "ERROR_INVALID_SERVER_NAME"},
        {Config::ERROR_INVALID_ALLOWED_METHODS, "ERROR_INVALID_ALLOWED_METHODS"},
        {Config::ERROR_INVALID_ROOT, "ERROR_INVALID_ROOT"},
        {Config::ERROR_INVALID_ERROR_PAGE, "ERROR_INVALID_ERROR_PAGE"},
        {Config::ERROR_INVALID_CLIENT_MAX_BODY_SIZE, "ERROR_INVALID_CLIENT_MAX_BODY_SIZE"},
        {Config::ERROR_INVALID_LOCATION, "ERROR_INVALID_LOCATION"},
        {Config::ERROR_INVALID_CGI_PATH, "ERROR_INVALID_CGI_PATH"},
        {Config::ERROR_INVALID_AUTOINDEX, "ERROR_INVALID_AUTOINDEX"},
        {Config::ERROR_INVALID_UPLOAD_PATH, "ERROR_INVALID_UPLOAD_PATH"},
        {Config::ERROR_INVALID_REDIRECT, "ERROR_INVALID_REDIRECT"},
        {Config::ERROR_INVALID_PREFIX, "ERROR_INVALID_PREFIX"}
    };
    map<Config::e_error, const char*>::const_iterator it = messages.find(error);
    return (it != messages.end()) ? it->second : "ERROR_UNKNOWN";
}

bool key_exists(const toml::Table &t, const string &key) {
    return t.mp.find(key) != t.mp.end();
}

bool optional_is(const toml::Table &t, const string &key, toml::Table::e_toml type) {
    return !key_exists(t, key) || t.at(key).is_type(type);
}

bool optional_str_array(const toml::Table &t, const string &key) {
    if (!optional_is(t, key, toml::Table::ARRAY)) return false;
    const toml::Table& arr = t.at(key);
    for (size_t i = 0; i < arr.vec.size(); ++i)
        if (!arr.vec[i].is_type(toml::Table::STRING)) return false;
    return true;
}

void fill_string_array(const toml::Table &t, const string &key, vector<string> &target, const vector<string> &fallback) {
    if (!key_exists(t, key) || t.at(key).is_type(toml::Table::NONE)) {
        target = fallback;
        return;
    }
    const toml::Table &arr = t.at(key);
    for (size_t i = 0; i < arr.vec.size(); ++i)
        target.push_back(arr.vec[i].as_str(""));
}

LocationConfig fill_location(const toml::Table &t, const ServerConfig &defaults) {
    LocationConfig loc;
    loc.prefix = t.at("prefix").as_str("/");
    loc.root = t.at("root").as_str(defaults.root);
    loc.autoindex = t.at("autoindex").as_str("off");
    loc.upload_path = t.at("upload_path").as_str(defaults.upload_path);
    loc.client_max_body_size = t.at("_clientMaxBodySize").as_str(defaults.client_max_body_size);

    fill_string_array(t, "index", loc.index, defaults.index);
    fill_string_array(t, "allowed_methods", loc.allowed_methods, defaults.allowed_methods);
    fill_string_array(t, "error_page", loc.error_page, defaults.error_page);
    fill_string_array(t, "redirect", loc.redirect, defaults.redirect);
    fill_string_array(t, "cgi_path", loc.cgi_path, vector<string>());

    return loc;
}

ServerConfig fill_server(const toml::Table &t) {
    ServerConfig s;
    s.port = t.at("port").as_int(80);
    s.host = t.at("host").as_str("127.0.0.1");
    s.root = t.at("root").as_str("www");
    s.upload_path = t.at("upload_path").as_str("www/upload");
    s.client_max_body_size = t.at("_clientMaxBodySize").as_str("1m");

    fill_string_array(t, "index", s.index, vector<string>());
    fill_string_array(t, "server_name", s.server_name, vector<string>());
    fill_string_array(t, "allowed_methods", s.allowed_methods, vector<string>());
    fill_string_array(t, "error_page", s.error_page, vector<string>());
    fill_string_array(t, "redirect", s.redirect, vector<string>());

    if (key_exists(t, "location") && t.at("location").is_type(toml::Table::ARRAY)) {
        const toml::Table &arr = t.at("location");
        for (size_t i = 0; i < arr.vec.size(); ++i)
            s.locations.push_back(fill_location(arr.vec[i], s));
    }
    return s;
}

Config::e_error validate_location(const toml::Table &t) {
    static const string allowed[] = {"prefix", "root", "upload_path", "autoindex", "redirect", "allowed_methods", "index", "cgi_path", "error_page", "_clientMaxBodySize"};
    for (map<string, toml::Table>::const_iterator it = t.mp.begin(); it != t.mp.end(); ++it)
        if (std::find(allowed, allowed + sizeof(allowed) / sizeof(*allowed), it->first) == allowed + sizeof(allowed) / sizeof(*allowed))
            return Config::ERROR_UNKNOWN_KEY;

    if (!optional_is(t, "prefix", toml::Table::STRING)) return Config::ERROR_INVALID_PREFIX;
    if (!optional_is(t, "root", toml::Table::STRING)) return Config::ERROR_INVALID_ROOT;
    if (!optional_is(t, "upload_path", toml::Table::STRING)) return Config::ERROR_INVALID_UPLOAD_PATH;
    if (!optional_is(t, "autoindex", toml::Table::STRING)) return Config::ERROR_INVALID_AUTOINDEX;
    if (!optional_is(t, "_clientMaxBodySize", toml::Table::STRING)) return Config::ERROR_INVALID_CLIENT_MAX_BODY_SIZE;
    if (!optional_str_array(t, "redirect")) return Config::ERROR_INVALID_REDIRECT;
    if (!optional_str_array(t, "cgi_path")) return Config::ERROR_INVALID_CGI_PATH;
    if (!optional_str_array(t, "index")) return Config::ERROR_INVALID_INDEX;
    if (!optional_str_array(t, "allowed_methods")) return Config::ERROR_INVALID_ALLOWED_METHODS;
    if (!optional_str_array(t, "error_page")) return Config::ERROR_INVALID_ERROR_PAGE;

    return Config::ERROR_NONE;
}

Config::e_error Config::pre_validate(toml::Table &config) {
    if (!key_exists(config, "server") || !config["server"].is_type(toml::Table::ARRAY))
        return Config::ERROR_INVALID_SERVER;

    const toml::Table &arr = config["server"];
    for (size_t i = 0; i < arr.vec.size(); ++i) {
        Config::e_error err = validate_server(arr.vec[i]);
        if (err != Config::ERROR_NONE)
            return err;
    }
    return Config::ERROR_NONE;
}

Config::e_error Config::validate_server(const toml::Table &t) {
    static const string allowed[] = {"port", "host", "index", "server_name", "allowed_methods", "root", "error_page", "_clientMaxBodySize", "location", "upload_path", "redirect"};
    for (map<string, toml::Table>::const_iterator it = t.mp.begin(); it != t.mp.end(); ++it)
        if (std::find(allowed, allowed + sizeof(allowed) / sizeof(*allowed), it->first) == allowed + sizeof(allowed) / sizeof(*allowed))
            return Config::ERROR_UNKNOWN_KEY;

    if (!optional_is(t, "port", toml::Table::STRING)) return Config::ERROR_INVALID_PORT;
    if (!optional_is(t, "upload_path", toml::Table::STRING)) return Config::ERROR_INVALID_UPLOAD_PATH;
    if (!optional_is(t, "host", toml::Table::STRING)) return Config::ERROR_INVALID_HOST;
    if (!optional_is(t, "_clientMaxBodySize", toml::Table::STRING)) return Config::ERROR_INVALID_CLIENT_MAX_BODY_SIZE;
    if (!optional_str_array(t, "redirect")) return Config::ERROR_INVALID_REDIRECT;
    if (!optional_str_array(t, "index")) return Config::ERROR_INVALID_INDEX;
    if (!optional_str_array(t, "server_name")) return Config::ERROR_INVALID_SERVER_NAME;
    if (!optional_str_array(t, "allowed_methods")) return Config::ERROR_INVALID_ALLOWED_METHODS;
    if (!optional_is(t, "root", toml::Table::STRING)) return Config::ERROR_INVALID_ROOT;
    if (!optional_str_array(t, "error_page")) return Config::ERROR_INVALID_ERROR_PAGE;

    if (key_exists(t, "location") && t.at("location").is_type(toml::Table::ARRAY)) {
        const toml::Table &arr = t.at("location");
        for (size_t i = 0; i < arr.vec.size(); ++i) {
            Config::e_error err = validate_location(arr.vec[i]);
            if (err != Config::ERROR_NONE) return err;
        }
    }
    return Config::ERROR_NONE;
}

Config::e_error Config::post_validate() {
    if (_servers.empty())
        return Config::ERROR_INVALID_SERVER;
    for (size_t i = 0; i < _servers.size(); ++i) {
        if (_servers[i].port < 0 || _servers[i].port > 65535)
            return Config::ERROR_INVALID_PORT;
        const vector<string> &err_pages = _servers[i].error_page;
        if (err_pages.size() < 2 && !err_pages.empty())
            return Config::ERROR_INVALID_ERROR_PAGE;
        for (size_t j = 0; j + 1 < err_pages.size(); j++) {
            int code = atoi(err_pages[j].c_str());
            if (code < 100 || code > 599)
                return Config::ERROR_INVALID_ERROR_PAGE;
        }
    }
    return Config::ERROR_NONE;
}

Config::Config(toml::Table &config) {
    error = pre_validate(config);
    if (error != ERROR_NONE) fatal("Invalid Config file");
    const toml::Table &servers = config["server"];
    for (size_t i = 0; i < servers.vec.size(); ++i)
        _servers.push_back(fill_server(servers.vec[i]));
    error = post_validate();
    if (error != ERROR_NONE) fatal("Invalid Config file");
}

void Config::fatal(const char *msg) {
    print();
    cout << "ERROR: " << msg << endl;
    exit(1);
}

void Config::print() {
    cout << "\n<==================== Config ====================>" << endl;
    for (size_t i = 0; i < _servers.size(); i++)
        _servers[i].print();
    cout << get_error_message(error) << endl;
    cout << "\n<==================== END Config ====================>\n" << endl;
}

const vector<ServerConfig>& Config::getServers() const {
    return _servers;
}

}
