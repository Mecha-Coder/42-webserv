#include "../include/webserv.hpp"

void mimeType(std::map<Str, Str> &mime)
{
    // Text types
    mime.insert(std::make_pair("txt", "text/plain"));
    mime.insert(std::make_pair("html", "text/html"));
    mime.insert(std::make_pair("css", "text/css"));
    mime.insert(std::make_pair("xml", "text/xml"));
    mime.insert(std::make_pair("htc", "text/x-component"));
    
    // Image types
    mime.insert(std::make_pair("gif", "image/gif"));
    mime.insert(std::make_pair("jpeg", "image/jpeg"));
    mime.insert(std::make_pair("jpg", "image/jpeg"));
    mime.insert(std::make_pair("avif", "image/avif"));
    mime.insert(std::make_pair("png", "image/png"));
    mime.insert(std::make_pair("svg", "image/svg+xml"));
    mime.insert(std::make_pair("tif", "image/tiff"));
    mime.insert(std::make_pair("wbmp", "image/vnd.wap.wbmp"));
    mime.insert(std::make_pair("webp", "image/webp"));
    mime.insert(std::make_pair("ico", "image/x-icon"));
    mime.insert(std::make_pair("jng", "image/x-jng"));
    mime.insert(std::make_pair("bmp", "image/x-ms-bmp"));
    
    // Application types
    mime.insert(std::make_pair("js", "application/javascript"));
    mime.insert(std::make_pair("json", "application/json"));
    mime.insert(std::make_pair("doc", "application/msword"));
    mime.insert(std::make_pair("pdf", "application/pdf"));
    mime.insert(std::make_pair("xls", "application/vnd.ms-excel"));
    mime.insert(std::make_pair("ppt", "application/vnd.ms-powerpoint"));
    mime.insert(std::make_pair("pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"));
    mime.insert(std::make_pair("xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"));
    mime.insert(std::make_pair("docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"));
    mime.insert(std::make_pair("wmlc", "application/vnd.wap.wmlc"));
    mime.insert(std::make_pair("7z", "application/x-7z-compressed"));
    mime.insert(std::make_pair("run", "application/x-makeself"));
    mime.insert(std::make_pair("pl", "application/x-perl"));
    mime.insert(std::make_pair("rar", "application/x-rar-compressed"));
    mime.insert(std::make_pair("swf", "application/x-shockwave-flash"));
    mime.insert(std::make_pair("xhtml", "application/xhtml+xml"));
    mime.insert(std::make_pair("zip", "application/zip"));
    
    // Binary/octet-stream types
    mime.insert(std::make_pair("bin", "application/octet-stream"));
    mime.insert(std::make_pair("exe", "application/octet-stream"));
    mime.insert(std::make_pair("dll", "application/octet-stream"));
    mime.insert(std::make_pair("deb", "application/octet-stream"));
    mime.insert(std::make_pair("dmg", "application/octet-stream"));
    mime.insert(std::make_pair("iso", "application/octet-stream"));
    mime.insert(std::make_pair("msi", "application/octet-stream"));
    
    // Audio types
    mime.insert(std::make_pair("mp3", "audio/mpeg"));
    mime.insert(std::make_pair("m4a", "audio/x-m4a"));
    mime.insert(std::make_pair("ra", "audio/x-realaudio"));
    
    // Video types
    mime.insert(std::make_pair("3gp", "video/3gpp"));
    mime.insert(std::make_pair("ts", "video/mp2t"));
    mime.insert(std::make_pair("mp4", "video/mp4"));
    mime.insert(std::make_pair("mpeg", "video/mpeg"));
    mime.insert(std::make_pair("mov", "video/quicktime"));
    mime.insert(std::make_pair("webm", "video/webm"));
    mime.insert(std::make_pair("flv", "video/x-flv"));
    mime.insert(std::make_pair("m4v", "video/x-m4v"));
    mime.insert(std::make_pair("wmv", "video/x-ms-wmv"));
    mime.insert(std::make_pair("avi", "video/x-msvideo"));
}

Str getContentType(const Str &filename)
{
    std::map<Str, Str> mime; 
    
    mimeType(mime);
    size_t pos = filename.find(".");
    if (pos != filename.npos)
    {
        Str type = mime[filename.substr(pos + 1)];
        if (!type.empty()) return type;
    }
    return "text/plain";
}