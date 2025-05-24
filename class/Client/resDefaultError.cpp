#include "../../include/Client.hpp"

Str getCodeShText(int code);
Str getCodeLgText(int code);

Str getHTML(const Str& errorCode, const Str & errorTitle, const Str& errorMsg)
{
    Str body =
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "    <meta charset=\"UTF-8\">\n"
        "    <meta name=\"" + errorCode +  "\" content=\"width=device-width, initial-scale=1.0\">\n"
        "    <title>" + errorCode + " " + errorTitle + "</title>\n"
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
        "    </style>\n"
        "</head>\n"
        "<body>\n"
        "    <div class=\"error-code\">" + errorCode +"</div>\n"
        "    <div class=\"error-title\">" + errorTitle + "</div>\n"
        "    <div class=\"error-message\">\n"
        "        " +  errorMsg + ".\n"
        "    </div>\n"
        "    <div>Go to <a href=\"/\">Home Page</a></div>\n"
        "</body>\n"
        "</html>\n";

    std::ostringstream oss;
    oss << body.size();

    Str head =
        "HTTP/1.1 " + errorCode + " " + errorTitle + "\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + oss.str() + "\r\n\r\n";
    
    return (head + body);
}

void Client::resDefaultError(int code)
{
     std::ostringstream oss;
    oss << code;

    Str result = getHTML(oss.str(), getCodeShText(code), getCodeLgText(code));
    this->reply.insert(this->reply.end(), result.begin(), result.end());
}