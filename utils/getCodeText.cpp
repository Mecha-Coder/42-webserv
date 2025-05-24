#include "../include/webserv.hpp"

Str getCodeShText(int code)
{
    switch(code) 
    {
        case 200: return "OK";
        case 201: return "Created";
        case 204: return "No Content";
        case 301: return "Moved Permanently";
        case 308: return "Permanent Redirect";
        case 400: return "Bad Request";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 410: return "Gone";
        case 413: return "Content Too Large";
        case 500: return "Internal Server Error";
    }
    return "";
}


Str getCodeLgText(int code)
{
    switch(code)
    {
        case 200: return "The request has succeeded";
        case 201: return "The request has been fulfilled and resulted in a new resource being created";
        case 204: return "The server successfully processed the request, but is not returning any content";
        case 301: return "The requested resource has been permanently moved to a new URI";
        case 308: return "The resource is now permanently located at another URI, and the request should be repeated using another URI";
        case 400: return "The server could not understand the request due to invalid syntax";
        case 403: return "The server understood the request but refuses to authorize it";
        case 404: return "The requested resource could not be found on the server";
        case 405: return "The request method is not supported for the requested resource";
        case 410: return "The resource requested is no longer available and will not be available again";
        case 413: return "The request entity is larger than the server is willing or able to process";
        case 500: return "The server encountered an unexpected condition that prevented it from fulfilling the request";
    }
    return "";
}
