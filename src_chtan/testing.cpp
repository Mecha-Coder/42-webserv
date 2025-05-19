#include <iostream>
#include <string>
#include <map>
#include <dirent.h>  // For directory operations
#include <sys/stat.h> // For stat()
#include <cerrno>    // For errno
#include <cstring>   // For strerror()
#include <cstdlib>
#include <unistd.h> // For close()
#include <filesystem>
#include <string>
#include <vector>

std::vector<std::string> read_dir(const std::string &path)
{
    std::vector<std::string> result;
    DIR* dir = opendir(path.c_str());
    if (!dir) {
        std::cerr << "Error opening directory: " << strerror(errno) << std::endl;
        return result;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        result.push_back(entry->d_name);
    }

    closedir(dir);
    // for (std::vector<std::string>::iterator it = result.begin(); it != result.end(); ++it) {
    //     std::cout << *it << std::endl;
    // }
    return result;
}

std::string get_html_template(const std::string& directory_name, const std::vector<std::string>& files)
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

void testing(){
    std::vector<std::string> files = read_dir("./website");
    std::string tmp = get_html_template("website", files);
    std::cout << tmp << std::endl;
}

int main(void)
{
    testing();
    
    return 0;
}