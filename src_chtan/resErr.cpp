/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resErr.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chtan <chtan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:10:27 by chtan             #+#    #+#             */
/*   Updated: 2025/05/22 15:02:57 by chtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"

void    respond_error(int error)
{
    ;
}

std::string    respond_default(void)
{
    bool check = false;
    search("website", "main.html", check);
    return read_file("website/main.html");
    // return tmp;
}

// std::vector<std::string> read_dir(const std::string &path)
// {
//     std::vector<std::string> result;
//     DIR* dir = opendir(path.c_str());
//     if (!dir) {
//         std::cerr << "Error opening directory: " << strerror(errno) << std::endl;
//         return result;
//     }

//     struct dirent* entry;
//     while ((entry = readdir(dir)) != NULL) {
//         // Skip "." and ".." entries
//         if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
//             continue;

//         result.push_back(entry->d_name);
//     }

//     closedir(dir);
//     // for (std::vector<std::string>::iterator it = result.begin(); it != result.end(); ++it) {
//     //     std::cout << *it << std::endl;
//     // }
//     return result;
// }

std::string template_html(const std::string& error_code, const std::string& error_type, const std::string& error_type2)
{
    std::string body = 
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "    <meta charset=\"UTF-8\">\n"
        "    <meta name=\"" + error_code + "\" content=\"width=device-width, initial-scale=1.0\">\n"
        "    <title>" + error_code + error_type + "</title>\n"
        "    <style>\n"
        "        body {\n"
        "            font-family: Arial, sans-serif;\n"
        "            text-align: center;\n"
        "            margin-top: 50px;\n"
        "        }\n"
        "        h1 {\n"
        "            color: #d9534f;\n"
        "        }\n"
        "        #datetime {\n"
        "            margin-top: 30px;\n"
        "            font-size: 18px;\n"
        "            color: #555;\n"
        "        }\n"
        "        a {\n"
        "            display: inline-block;\n"
        "            margin-top: 20px;\n"
        "            color: #0066cc;\n"
        "            text-decoration: none;\n"
        "        }\n"
        "    </style>\n"
        "</head>\n"
        "<body>\n"
        "    <h1>" + error_code +"-" + error_type + "</h1>\n"
        "    <p>" + error_type2 + "</p>\n"
        "    <a href=\"/\">Return to Home Page</a>\n"
        "</body>\n"
        "</html>\n";

    std::string head =
        "HTTP/1.1" + error_code + error_type + "\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + std::to_string(body.size()) + "\r\n]\r\n";
    std::string ret = head + body;
}

std::string res_dir_tmp(const std::string& directory_name, const std::vector<std::string>& files)
{    
    std::string html = 
            "<!DOCTYPE html>\n"
            "<html lang=\"en\">\n"
            "<head>\n"
            "  <meta charset=\"UTF-8\">\n"
            "  <title>" + directory_name + "</title>\n"
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
            "  <h1>" + directory_name + "</h1>\n"
            "  <hr>\n"
            "  <pre>\n"
            "<a href=\"../\">../</a>\n";

            for (size_t i = 0; i < files.size(); ++i) {
                html += "<a href=\"" + files[i] + "/\">" + files[i] + "/</a>\n";
            }
            html += "  </pre>\n"
            "  <hr>\n"
            "</body>\n"
            "</html>\n";
        std::string head =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: " + std::to_string(html.size()) + "\r\n";
        std::string ret = head + html;
        return ret;
}

void default_templete()
{
    std::vector<std::string> files = read_dir("./website");
    std::string tmp = res_dir_tmp("website", files);
    std::cout << tmp << std::endl;
}

int main(void)
{
    // response res;

    std::string tmp = respond_default();
    std::cout << tmp << std::endl;

    return 0;
}
