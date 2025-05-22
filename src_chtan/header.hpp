/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chtan <chtan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:46:44 by chtan             #+#    #+#             */
/*   Updated: 2025/05/22 14:44:20 by chtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <map>
#include <dirent.h>  // For directory operations
#include <sys/stat.h> // For stat()
#include <cerrno>    // For errno
#include <cstring>   // For strerror()
#include <cstdlib>
#include <unistd.h> // For close()
#include <filesystem>
#include <string>
#include <fstream>
#include <vector>

typedef std::string str;
namespace fs = std::filesystem;
enum nb {
    ERROR_505 = 505,
    ERROR_404 = 404
};

typedef enum error_code
{
    ERROR_200 = 200,
    ERROR_201 = 201,
    ERROR_202 = 202,
    ERROR_204 = 204,

    ERROR_301 = 301,
    ERROR_302 = 302,

    ERROR_400 = 400,
    ERROR_401 = 401,
    ERROR_403 = 403,
    ERROR_404 = 404,
    ERROR_405 = 405,
    ERROR_408 = 408,
    ERROR_414 = 414,
    ERROR_429 = 429,

    ERROR_500 = 500,
    ERROR_501 = 501,
    ERROR_502 = 502,
    ERROR_503 = 503,
    ERROR_505 = 505,
} error_code;

class header {
    private:
        struct ErrorDescription {
            std::string title;
            std::string description;
        };

        static const std::pair<error_code, ErrorDescription> errorDescriptions[];

    protected:
        error_code err;

    public:
        header(void);
        header(const header &tocopy);
        header &operator=(const header &tocopy);
        ~header(void);
        std::string get_map(int error) const;
};

const std::pair<error_code, header::ErrorDescription> header::errorDescriptions[] = {
    std::make_pair(ERROR_200, ErrorDescription{"OK", "The request was successful."}),
    std::make_pair(ERROR_201, ErrorDescription{"Created", "The request was successful and a resource was created."}),
    std::make_pair(ERROR_202, ErrorDescription{"Accepted", "The request has been accepted for processing, but not completed."}),
    std::make_pair(ERROR_204, ErrorDescription{"No Content", "The server successfully processed the request, but is not returning any content."}),

    std::make_pair(ERROR_301, ErrorDescription{"Moved Permanently", "The resource has been moved to a new URL permanently."}),
    std::make_pair(ERROR_302, ErrorDescription{"Found", "The resource resides temporarily under a different URL."}),

    std::make_pair(ERROR_400, ErrorDescription{"Bad Request", "The server could not understand the request."}),
    std::make_pair(ERROR_401, ErrorDescription{"Unauthorized", "Authentication is required and has failed or has not been provided."}),
    std::make_pair(ERROR_403, ErrorDescription{"Forbidden", "The server understood the request but refuses to authorize it."}),
    std::make_pair(ERROR_404, ErrorDescription{"Not Found", "The requested resource could not be found."}),
    std::make_pair(ERROR_405, ErrorDescription{"Method Not Allowed", "The HTTP method is not allowed for the requested resource."}),
    std::make_pair(ERROR_408, ErrorDescription{"Request Timeout", "The server timed out waiting for the request."}),
    std::make_pair(ERROR_414, ErrorDescription{"URI Too Long", "The URI provided was too long for the server to process."}),
    std::make_pair(ERROR_429, ErrorDescription{"Too Many Requests", "The user has sent too many requests in a given amount of time."}),

    std::make_pair(ERROR_500, ErrorDescription{"Internal Server Error", "A generic error occurred on the server."}),
    std::make_pair(ERROR_501, ErrorDescription{"Not Implemented", "The server does not support the functionality required."}),
    std::make_pair(ERROR_502, ErrorDescription{"Bad Gateway", "The server received an invalid response from an upstream server."}),
    std::make_pair(ERROR_503, ErrorDescription{"Service Unavailable", "The server is currently unavailable (overloaded or down)."}),
    std::make_pair(ERROR_505, ErrorDescription{"HTTP Version Not Supported", "The server does not support the HTTP version used in the request."})
};

class response
{
    private:
        std::string _status;
        int _content_type;// 1 for file and 0 for dir
        std::string _path;
        std::string _content_length;
        std::string _content;
        // std::string _body;
        // std::string _type;

    public:
        response(void);
        response(const response &tocopy);
        response &operator=(const response &tocopy);
        ~response(void);

        void set_status(const std::string& status);
        void set_content(const int content_type);
        void set_content_length(const std::string& len);

        std::string get_status() const;
        int         get_content_type() const;
        std::string get_content_length() const;
};

// class client
// {
//     protected:
//         std::string _name;
//         std::string _ip;
//         int         _port;


//     public:

// };
// get path
// void            resDirlist(void);

//check_dir.cpp
std::string                 listDirectoryPOSIX(const std::string& path);
void                        checkPathAndSetResponse(const std::string& path, response& res);
std::string*                search(const std::string& path, const std::string& filename, bool &check);
std::string                 search_recursive(const std::string& dir, const std::string& filename);
std::vector<std::string>    read_dir(const std::string &path);
std::string                 read_file(const std::string& filePath);


//resErr.cpp
std::string                 respond_default(void);
void                        respond_error(int error);
void                        resDirlist(response &res);
std::string                 res_dir_tmp(const std::string& directory_name, const std::vector<std::string>& files);
void                        default_templete();
