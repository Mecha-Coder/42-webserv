#include "../../include/Client.hpp"

bool readDir(const Str &path, DirItems &items)
{
    DIR* dir = opendir(path.c_str());
    if (!dir) return false;

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) 
    {
        // Skip (".")  ("..") (symbolic link) and (unknown)
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 
            || entry->d_type == DT_LNK || entry->d_type == DT_UNKNOWN)
            continue;

        items.insert(std::make_pair(entry->d_name, (entry->d_type == DT_DIR)));
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
        "      font-family: \"Segoe UI\", Tahoma, Geneva, Verdana, sans-serif;\n"
        "      background-color: #f9f9f9;\n"
        "      color: #333;\n"
        "      padding: 20px;\n"
        "    }\n"
        "\n"
        "    h1 {\n"
        "      font-size: 2em;\n"
        "      margin-bottom: 10px;\n"
        "    }\n"
        "\n"
        "    hr {\n"
        "      border: none;\n"
        "      border-top: 1px solid #ccc;\n"
        "      margin: 20px 0;\n"
        "    }\n"
        "\n"
        "    table {\n"
        "      width: 100%;\n"
        "      border-collapse: collapse;\n"
        "    }\n"
        "\n"
        "    th, td {\n"
        "      text-align: left;\n"
        "      padding: 8px 12px;\n"
        "    }\n"
        "\n"
        "    th {\n"
        "      background-color: #f2f2f2;\n"
        "      border-bottom: 2px solid #ddd;\n"
        "    }\n"
        "\n"
        "    tr:nth-child(even) {\n"
        "      background-color: #fdfdfd;\n"
        "    }\n"
        "\n"
        "    tr:hover {\n"
        "      background-color: #f1f1f1;\n"
        "    }\n"
        "\n"
        "    a {\n"
        "      color: #0073e6;\n"
        "      text-decoration: none;\n"
        "    }\n"
        "\n"
        "    a:hover {\n"
        "      text-decoration: underline;\n"
        "    }\n"
        "  </style>\n"
        "</head>\n"
        "<body>\n"
        "  <h1>Index of " + route + "</h1>\n"
        "  <hr>\n"
        "  <table>\n"
        "    <thead>\n"
        "      <tr>\n"
        "        <th>Name</th>\n"
        "      </tr>\n"
        "    </thead>\n"
        "    <tbody>\n"
        "      <tr><td><a href=\"../\">../</a></td></tr>\n";

    DirItems::const_iterator i = items.begin();
    for (; i != items.end(); ++i)
    {
        if (i->second) body += "      <tr><td><a href=\"" + i->first + "/\">subfolder/</a></td></tr>\n";
        else           body += "      <tr><td><a href=\"" + i->first + "\">404.css</a></td></tr>\n";
    }

    body += 
        "    </tbody>\n"
        "  </table>\n"
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
