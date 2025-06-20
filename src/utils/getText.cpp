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


Str getContentType(const Str &filename)
{
    size_t pos = filename.find(".");    
    if (pos != filename.npos)
    {
        Str extension = filename.substr(pos + 1);

            // Text types
            if (extension == "txt") return "text/plain";
            if (extension == "html") return "text/html";
            if (extension == "css") return "text/css";
            if (extension == "xml") return "text/xml";
            if (extension == "htc") return "text/x-component";

            // Image types
            if (extension == "gif") return "image/gif";
            if (extension == "jpeg") return "image/jpeg";
            if (extension == "jpg") return "image/jpeg";
            if (extension == "avif") return "image/avif";
            if (extension == "png") return "image/png";
            if (extension == "svg") return "image/svg+xml";
            if (extension == "tif") return "image/tiff";
            if (extension == "wbmp") return "image/vnd.wap.wbmp";
            if (extension == "webp") return "image/webp";
            if (extension == "ico") return "image/x-icon";
            if (extension == "jng") return "image/x-jng";
            if (extension == "bmp") return "image/x-ms-bmp";

            // Application types
            if (extension == "js") return "application/javascript";
            if (extension == "json") return "application/json";
            if (extension == "doc") return "application/msword";
            if (extension == "pdf") return "application/pdf";
            if (extension == "xls") return "application/vnd.ms-excel";
            if (extension == "ppt") return "application/vnd.ms-powerpoint";
            if (extension == "pptx") return "application/vnd.openxmlformats-officedocument.presentationml.presentation";
            if (extension == "xlsx") return "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
            if (extension == "docx") return "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
            if (extension == "wmlc") return "application/vnd.wap.wmlc";
            if (extension == "7z") return "application/x-7z-compressed";
            if (extension == "run") return "application/x-makeself";
            if (extension == "pl") return "application/x-perl";
            if (extension == "rar") return "application/x-rar-compressed";
            if (extension == "swf") return "application/x-shockwave-flash";
            if (extension == "xhtml") return "application/xhtml+xml";
            if (extension == "zip") return "application/zip";

            // Binary/octet-stream types
            if (extension == "bin") return "application/octet-stream";
            if (extension == "exe") return "application/octet-stream";
            if (extension == "dll") return "application/octet-stream";
            if (extension == "deb") return "application/octet-stream";
            if (extension == "dmg") return "application/octet-stream";
            if (extension == "iso") return "application/octet-stream";
            if (extension == "msi") return "application/octet-stream";

            // Audio types
            if (extension == "mp3") return "audio/mpeg";
            if (extension == "m4a") return "audio/x-m4a";
            if (extension == "ra") return "audio/x-realaudio";

            // Video types
            if (extension == "3gp") return "video/3gpp";
            if (extension == "ts") return "video/mp2t";
            if (extension == "mp4") return "video/mp4";
            if (extension == "mpeg") return "video/mpeg";
            if (extension == "mov") return "video/quicktime";
            if (extension == "webm") return "video/webm";
            if (extension == "flv") return "video/x-flv";
            if (extension == "m4v") return "video/x-m4v";
            if (extension == "wmv") return "video/x-ms-wmv";
            if (extension == "avi") return "video/x-msvideo";

    }
    return "text/plain";
}