#include "../include/webserv.hpp"

Str statusBuilder(int code)
{
    Str statusLine = "HTTP/1.1 ";
    switch(code) 
    {
        case 200: return statusLine + "200 OK\r\n";
        case 201: return statusLine + "201 Created\r\n";
        case 204: return statusLine + "204 No Content\r\n";
        case 301: return statusLine + "301 Moved Permanently\r\n";
        case 308: return statusLine + "308 Permanent Redirect\r\n";
        case 400: return statusLine + "400 Bad Request\r\n";
        case 403: return statusLine + "403 Forbidden\r\n";
        case 404: return statusLine + "404 Not Found\r\n";
        case 405: return statusLine + "405 Method Not Allowed\r\n";
        case 410: return statusLine + "410 Gone\r\n";
        case 413: return statusLine + "413 Content Too Large\r\n";
        case 500: return statusLine + "500 Internal Server Error\r\n";
        default:
            return statusLine + "500 Internal Server Error\r\n";
    }
    return statusLine;
}
