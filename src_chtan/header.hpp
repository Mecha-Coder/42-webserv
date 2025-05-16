/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chtan <chtan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:46:44 by chtan             #+#    #+#             */
/*   Updated: 2025/05/16 22:20:15 by chtan            ###   ########.fr       */
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

// enum nb {
//     ERROR_505 = 505,
//     ERROR_404 = 404
// };

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
};

class header{
    private:
    const std::map<error_code, std::pair<std::string, std::string>> errorDescriptions = {
        {ERROR_200, {"OK", "The request was successful."}},
        {ERROR_201, {"Created", "The request was successful and a resource was created."}},
        {ERROR_202, {"Accepted", "The request has been accepted for processing, but not completed."}},
        {ERROR_204, {"No Content", "The server successfully processed the request, but is not returning any content."}},
    
        {ERROR_301, {"Moved Permanently", "The resource has been moved to a new URL permanently."}},
        {ERROR_302, {"Found", "The resource resides temporarily under a different URL."}},
    
        {ERROR_400, {"Bad Request", "The server could not understand the request."}},
        {ERROR_401, {"Unauthorized", "Authentication is required and has failed or has not been provided."}},
        {ERROR_403, {"Forbidden", "The server understood the request but refuses to authorize it."}},
        {ERROR_404, {"Not Found", "The requested resource could not be found."}},
        {ERROR_405, {"Method Not Allowed", "The HTTP method is not allowed for the requested resource."}},
        {ERROR_408, {"Request Timeout", "The server timed out waiting for the request."}},
        {ERROR_414, {"URI Too Long", "The URI provided was too long for the server to process."}},
        {ERROR_429, {"Too Many Requests", "The user has sent too many requests in a given amount of time."}},
    
        {ERROR_500, {"Internal Server Error", "A generic error occurred on the server."}},
        {ERROR_501, {"Not Implemented", "The server does not support the functionality required."}},
        {ERROR_502, {"Bad Gateway", "The server received an invalid response from an upstream server."}},
        {ERROR_503, {"Service Unavailable", "The server is currently unavailable (overloaded or down)."}},
        {ERROR_505, {"HTTP Version Not Supported", "The server does not support the HTTP version used in the request."}}
    };

    protected:
        error_code err;
        std::string err_msg(int i);

    public:
        header(void);
        header(const header &tocopy);
        header &operator=(const header &tocopy);
        ~header(void);
        std::string get_map(int error) const;
        
};

// class client
// {
//     protected:
//         std::string _name;
//         std::string _ip;
//         int         _port;


//     public:

// };

void    listDirectoryPOSIX(const std::string& path);
