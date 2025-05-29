#include "../include/webserv.hpp"

Str getCodeShText(Code code)
{
    switch(code) 
    {
        case _200: return "OK";
        case _201: return "Created";
        case _204: return "No Content";
        case _301: return "Moved Permanently";
        case _308: return "Permanent Redirect";
        case _400: return "Bad Request";
        case _403: return "Forbidden";
        case _404: return "Not Found";
        case _405: return "Method Not Allowed";
        case _410: return "Gone";
        case _413: return "Content Too Large";
        case _500: return "Internal Server Error";
    }
    return "";
}


Str getCodeLgText(Code code)
{
    switch(code)
    {
        case _200: return "The request has succeeded";
        case _201: return "The request has been fulfilled and resulted in a new resource being created";
        case _204: return "The server successfully processed the request, but is not returning any content";
        case _301: return "The requested resource has been permanently moved to a new URI";
        case _308: return "The resource is now permanently located at another URI, and the request should be repeated using another URI";
        case _400: return "The server could not understand the request due to invalid syntax";
        case _403: return "The server understood the request but refuses to authorize it";
        case _404: return "The requested resource could not be found on the server";
        case _405: return "The request method is not supported for the requested resource";
        case _410: return "The resource requested is no longer available and will not be available again";
        case _413: return "The request entity is larger than the server is willing or able to process";
        case _500: return "The server encountered an unexpected condition that prevented it from fulfilling the request";
    }
    return "";
}
