#include "../../include/Client.hpp"

Str getCodeShText(Code code);
Str getCodeLgText(Code code);
Str getContentType(const Str &filename);

/////////////////////////////////////////////////////////////////////////////////////

Str Client::tmplErrDefault(Code code)
{
    Str errCode  = toStr(code);
    Str errTitle = getCodeShText(code); 
    Str errMsg   = getCodeLgText(code);

    Str body =
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "    <meta charset=\"UTF-8\">\n"
        "    <meta name=\"" + errCode +  "\" content=\"width=device-width, initial-scale=1.0\">\n"
        "    <title>" + errCode + " " + errTitle + "</title>\n"
        "    <style>\n"
        "        body {\n"
        "            font-family: Arial, sans-serif;\n"
        "            text-align: center;\n"
        "            margin-top: 100px;\n"
        "            color: #000;\n"
        "        }\n"
        "        .error-code {\n"
        "            font-size: 100px;\n"
        "            font-weight: bold;\n"
        "            color: #d9534f;\n"
        "        }\n"
        "        .error-title {\n"
        "            font-size: 32px;\n"
        "            font-weight: bold;\n"
        "        }\n"
        "        .error-message {\n"
        "            font-size: 16px;\n"
        "            margin-top: 20px;\n"
        "        }\n"
        "        a {\n"
        "            color: #800080;\n"
        "            text-decoration: underline;\n"
        "        }\n"
        "        footer {\n"
        "           background-color: #f2f2f2;\n"
        "           padding: 20px;\n"
        "           text-align: center;\n"
        "           color: #555;\n"
        "           position: fixed;\n"
        "           bottom: 0;\n"
        "           width: 100%;\n"
        "       }"
        "    </style>\n"
        "</head>\n"
        "<body>\n"
        "    <div class=\"error-code\">" + errCode +"</div>\n"
        "    <div class=\"error-title\">" + errTitle + "</div>\n"
        "    <div class=\"error-message\">\n"
        "        " +  errMsg + ".\n"
        "    </div>\n"
        "    <div>Go to <a href=\"/\">Home Page</a></div>\n"
        "    <footer> &copy. Webserve default error page</footer>"
        "</body>\n"
        "</html>\n";

    Str head =
        "HTTP/1.1 " + errCode + " " + errTitle + "\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + toStr(body.size()) + "\r\n\r\n";
    
    return (head + body);
}

/////////////////////////////////////////////////////////////////////////////////////

Str Client::tmplErrCustom(Code code, const Str &body)
{
    Str head = 
        "HTTP/1.1 " + toStr(code) + " " + getCodeShText(code) + "\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + toStr(body.size()) + "\r\n\r\n";
    
    return (head + body);
}

/////////////////////////////////////////////////////////////////////////////////////

Str Client::tmplDirList(const Str &path, const DirItems &items)
{
	 Str body = 
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "  <meta charset=\"UTF-8\">\n"
        "  <title>" + path + "</title>\n"
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
        "  <h1>Index of " + path + "</h1>\n"
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
        if (i->second) 
            body += "      <tr><td><a href=\"" + i->first + "/\">" + i->first + "/</a></td></tr>\n";
        else           
            body += "      <tr><td><a href=\"" + i->first + "\">" + i->first + "</a></td></tr>\n";
    }

    body += 
        "    </tbody>\n"
        "  </table>\n"
        "  <hr>\n"
        "</body>\n"
        "</html>\n";

    Str head =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + toStr(body.size()) + "\r\n\r\n";

    return (head + body);
}

/////////////////////////////////////////////////////////////////////////////////////

Str Client::tmplFetch(const Str &filename, const Str &body)
{
    Str head = 
        "HTTP/1.1 200 OK \r\n"
        "Content-Disposition: inline; filename=\"" + filename + "\"\r\n"
        "Content-Type: " + getContentType(filename) + "\r\n"
        "Content-Length: " + toStr(body.size()) + "\r\n\r\n";
    
    return (head + body);
}

/////////////////////////////////////////////////////////////////////////////////////

Str Client::tmplSave(List saveFile)
{
    Str body = "{\n  \"status\": \"success\",\n  \"files\": [\n";

    List::const_iterator i = saveFile.begin();
    for (; i != saveFile.end(); ++i)
    {
        body += "    \"" + *i + "\"";
        if ((i + 1) != saveFile.end())
            body += ",";
        body += "\n";
    }
    body += "  ]\n}";

    Str head =
        "HTTP/1.1 201 Created\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: " + toStr(body.size()) + "\r\n\r\n";
    
    return (head + body);
}

/////////////////////////////////////////////////////////////////////////////////////

Str Client::tmplDelete(const Str &item)
{
    Str head = 
        "HTTP/1.1 204 No Content\r\n"
        "Content-Type : text/plain\r\n"
        "Content-Length: " + toStr(item.size()) + "\r\n\r\n";
    
    return (head + item);
}

/////////////////////////////////////////////////////////////////////////////////////

Str Client::tmplRedirect(Code code, const Str &redirectTo)
{
    Str head = 
        "HTTP/1.1 " + toStr(code) + " " + getCodeShText(code) + "\r\n"
        "Location: " + redirectTo + "\r\n"
        "Content-Length: 0\r\n\r\n";
    
    return head;
}

/////////////////////////////////////////////////////////////////////////////////////