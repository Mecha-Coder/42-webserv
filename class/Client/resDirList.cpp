#include "../../include/Client.hpp"

bool readDir(const Str &path, DirItems &items)
{
    DIR* dir = opendir(path.c_str());
    if (!dir) return false;

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) 
    {
        // Skip "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        items.push_back(entry->d_name);
    }
    closedir(dir);
    return true;
}

Str getHTML(const Str &route, const DirItems &items)
{
    Str body = 
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "  <meta charset=\"UTF-8\">\n"
        "  <title>" + route + "</title>\n"
        "  <style>\n"
        "    body {\n"
        "      font-family: sans-serif;\n"
        "      margin: 1em;\n"
        "    }\n"
        "    h1 {\n"
        "      font-size: 1.5em;\n"
        "      margin-bottom: 0.2em;\n"
        "    }\n"
        "    hr {\n"
        "      border: none;\n"
        "      border-top: 1px solid #aaa;\n"
        "      margin: 0.5em 0;\n"
        "    }\n"
        "    pre {\n"
        "      font-family: monospace;\n"
        "      line-height: 1.4;\n"
        "    }\n"
        "    pre a {\n"
        "      text-decoration: none;\n"
        "      color: #00f;\n"
        "    }\n"
        "    pre a:hover {\n"
        "      text-decoration: underline;\n"
        "    }\n"
        "  </style>\n"
        "</head>\n"
        "<body>\n"
        "  <h1>" + route + "</h1>\n"
        "  <hr>\n"
        "  <pre>\n"
        "<a href=\"../\">../</a>\n";

        for (size_t i = 0; i < items.size(); ++i)
        {
            body += "<a href=\"" + items[i] + "/\">" + items[i] + "/</a>\n";
        }
        body += "  </pre>\n"
        "  <hr>\n"
        "</body>\n"
        "</html>\n";

    std::ostringstream oss;
    oss << body.size();
    
    Str head =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + oss.str() + "\r\n\r\n";

    return (head + body);
}

void Client::resDirList()
{
    DirItems items;
    Str result;

    if (readDir(this->_filePath, items))
    {
        result = getHTML(this->_path, items);
        this->reply.insert(this->reply.end(), result.begin(), result.end());
    }
    else this->resError(500);
}
