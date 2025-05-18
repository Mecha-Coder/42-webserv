#pragma once

#include "Toml.hpp"
#include "../utils/Utils.hpp"
#include <string>
#include <vector>
#include <iostream>

typedef std::vector<std::string> string_vector;

struct LocationConfig {
    std::string prefix;
    std::string root;
    std::string autoindex;
    std::string upload_path;
    string_vector cgi_path;
    std::string client_max_body_size;
    string_vector error_page;
    string_vector redirect;
    string_vector index;
    string_vector allowed_methods;

    void print(int indent = 0) const;
};

struct ServerConfig {
    unsigned short port;
    std::string host;
    std::string root;
    std::string client_max_body_size;
    std::string upload_path;
    string_vector redirect;
    string_vector server_name;
    string_vector error_page;
    string_vector allowed_methods;
    string_vector index;
    std::vector<LocationConfig> locations;

    void print() const;
};

class Config {
public:
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

    Config(toml::Table& config);
    ~Config();

    const std::vector<ServerConfig>& getServers() const;
    void print() const;

    e_error error;

private:
    std::vector<ServerConfig> _servers;
    e_error pre_validate(toml::Table& config);
    e_error post_validate();
};
