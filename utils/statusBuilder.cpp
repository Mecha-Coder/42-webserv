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

std::pair<Str, Str> getCodeText(int code)
{
    switch(code)
    {
        case 200: return std::make_pair("OK", "The request has succeeded");
        case 201: return std::make_pair("Created", "The request has been fulfilled and resulted in a new resource being created");
        case 204: return std::make_pair("No Content", "The server successfully processed the request, but is not returning any content");
        case 301: return std::make_pair("Moved Permanently", "The requested resource has been permanently moved to a new URI");
        case 308: return std::make_pair("Permanent Redirect", "The resource is now permanently located at another URI, and the request should be repeated using another URI");
        case 400: return std::make_pair("Bad Request", "The server could not understand the request due to invalid syntax");
        case 403: return std::make_pair("Forbidden", "The server understood the request but refuses to authorize it");
        case 404: return std::make_pair("Not Found", "The requested resource could not be found on the server");
        case 405: return std::make_pair("Method Not Allowed", "The request method is not supported for the requested resource");
        case 410: return std::make_pair("Gone",	"The resource requested is no longer available and will not be available again");
        case 413: return std::make_pair("Content Too Large", "The request entity is larger than the server is willing or able to process");
        case 500: return std::make_pair("Internal Server Error", "The server encountered an unexpected condition that prevented it from fulfilling the request");
    }
    return std::make_pair("","");
}
