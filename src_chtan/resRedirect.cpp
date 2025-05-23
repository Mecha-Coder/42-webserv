/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resRedirect.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chtan <chtan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 10:50:40 by chtan             #+#    #+#             */
/*   Updated: 2025/05/23 11:03:22 by chtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "header.hpp"
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

std::string resRedirect(int statusCode, const std::string& redirectPath)
{
    // 1. Get redirect info from client.route
    int         stat = statusCode;
    std::string location   = redirectPath;

    // 2. Map status code to message
    std::string statusMsg;
    switch (statusCode) {
        case 301: statusMsg = "Moved Permanently"; break;
        case 302: statusMsg = "Found"; break;
        case 307: statusMsg = "Temporary Redirect"; break;
        case 308: statusMsg = "Permanent Redirect"; break;
        default:  statusMsg = "Redirect"; // Fallback
    }

    // 3. Build the HTTP response
    std::string response;
    response += "HTTP/1.1 " + std::to_string(statusCode) + " " + statusMsg + "\r\n";
    response += "Location: " + location + "\r\n";
    response += "Content-Length: 0\r\n";  // No body
    response += "Connection: close\r\n";  // Or keep-alive if needed
    response += "\r\n";  // End of headers

    return response;
}

int main()
{
    // Example usage
    int statusCode = 301;
    std::string redirectPath = "http://example.com/new-location";
    std::string response = resRedirect(statusCode, redirectPath);
    std::cout << response<< std::endl;

    return 0;
}