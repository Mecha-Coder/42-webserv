// #include "src_chtan/header.hpp"

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
#include <fstream>
#include <vector>


class response
{
    private:
        std::string _status;
        int _content_type;// 1 for file and 0 for dir
        std::string _path;
        std::string _content_length;
        std::string _content;
        // std::string _body;
        // std::string _type;

    public:
        response(void);
        response(const response &tocopy);
        response &operator=(const response &tocopy);
        ~response(void);

        void set_status(const std::string& status);
        void set_content(const int content_type);
        void set_content_length(const std::string& len);

        std::string get_status() const;
        int         get_content_type() const;
        std::string get_content_length() const;
};

/**
 * @brief Lists the contents of a directory in a POSIX-compliant way.
 * 
 * This function opens a directory specified by the path and lists its contents,
 * distinguishing between files and directories. It uses POSIX functions such as
 * opendir(), readdir(), and stat() to achieve this.
 * 
 * @param path The path to the directory to list.
 */
std::string listDirectoryPOSIX(const std::string& path) {
    if (path.empty()) {
        return "Error: Path is empty.";
    }

    DIR* dir = opendir(path.c_str());
    if (!dir) {
        return "Error opening directory: " + std::string(strerror(errno));
    }

    std::string result;
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        // Skip "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        std::string fullPath = path + "/" + entry->d_name;
        struct stat statBuf;
        if (stat(fullPath.c_str(), &statBuf) == -1) {
            result += std::string("Error stating file: ") + strerror(errno) + "\n";
            continue;
        }

        if (S_ISDIR(statBuf.st_mode)) {
            result += std::string(entry->d_name) + " [DIR]\n";
        } else {
            result += std::string(entry->d_name) + " (" + std::to_string(statBuf.st_size) + " bytes)\n";
        }
    }

    closedir(dir);
    return result.empty() ? "No entries found." : result;
}

// void checkPathAndSetResponse(const std::string& path, response& res) {
//     struct stat statBuf;
//     if (stat(path.c_str(), &statBuf) == -1) {
//         std::cerr << "Error: cannot stat path: " << strerror(errno) << std::endl;
//         res.set_status("Error: cannot access path");
//         return;
//     }

//     res.set_status(path);

//     if (S_ISREG(statBuf.st_mode)) {
//         res.set_content(1); // file
//         res.set_content_length(std::to_string(statBuf.st_size));
//     }
//     else if (S_ISDIR(statBuf.st_mode)) {
//         res.set_content(2); // directory
//         res.set_content_length("0");

//         std::string listing = listDirectoryPOSIX(path);
//         std::cout << "Directory contents:\n" << listing;
//         // Optionally store listing in the response object
//     }
//     else {
//         std::cerr << "Unsupported file type." << std::endl;
//         res.set_status("Unsupported file type");
//     }
// }

void search(const std::string& path, const std::string& filename)
{
    std::string* result = new std::string;
    DIR* dir = opendir(path.c_str());
    if (!dir) {
        std::cerr << "Error opening directory: " << strerror(errno) << std::endl;
        // return (nullptr);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::cout << entry->d_name << std::endl;
        if (strcmp(entry->d_name, filename.c_str()) == 0) {
            closedir(dir);
            // return path + "/" + entry->d_name;
        }
    }

    closedir(dir);
    // return (nullptr);
}

// std::string search_recursive(const std::string& dir, const std::string& filename) {
//     DIR* dp = opendir(dir.c_str());
//     if (!dp) {
//         std::cerr << "Error opening directory " << dir << ": " << strerror(errno) << std::endl;
//         return "";
//     }

//     struct dirent* entry;
//     while ((entry = readdir(dp)) != NULL) {
//         // Skip "." and ".."
//         if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
//             continue;
//         }

//         std::string full_path = dir + "/" + entry->d_name;

//         // Check if this is a directory
//         struct stat stat_buf;
//         if (stat(full_path.c_str(), &stat_buf) != 0) {
//             std::cerr << "Error stating " << full_path << ": " << strerror(errno) << std::endl;
//             continue;
//         }

//         if (S_ISDIR(stat_buf.st_mode)) {
//             // Recursively search subdirectory
//             std::string found = search_recursive(full_path, filename);
//             if (!found.empty()) {
//                 closedir(dp);
//                 return found;
//             }
//         } else if (S_ISREG(stat_buf.st_mode)) {
//             // Check if filename matches
//             if (strcmp(entry->d_name, filename.c_str()) == 0) {
//                 closedir(dp);
//                 return full_path;
//             }
//         }
//     }
// 
//     closedir(dp);
//     return "";
// }

std::string read_file(const std::string& filePath)
{
    std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary);
    
    if (!file) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return "";
    }
    
    // Determine file size
    file.seekg(0, std::ios::end);
    std::ifstream::pos_type fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // Read file contents into a vector
    std::vector<char> buffer(fileSize);
    file.read(&buffer[0], fileSize);
    
    // Convert to string
    return std::string(buffer.begin(), buffer.end());
}

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
    for (std::vector<std::string>::iterator it = result.begin(); it != result.end(); ++it) {
        std::cout << *it << std::endl;
    }
    return result;
}

// std::string listDirectoryPOSIX(const std::string& path) {
//     if(path.empty()) {
//         std::cerr << "Path is empty." << std::endl;
//         return;
//     }
//     DIR* dir = opendir(path.c_str());
//     if (!dir) {
//         std::cerr << "Error opening directory: " << strerror(errno) << std::endl;
//         return;
//     }

//     struct dirent* entry;
//     while ((entry = readdir(dir)) != nullptr) {
//         // Skip "." and ".." entries
//         if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
//             continue;

//         // Get full path for stat()
//         std::string fullPath = path + "/" + entry->d_name;
        
//         struct stat statBuf;
//         if (stat(fullPath.c_str(), &statBuf) == -1) {
//             std::cerr << "Error stating file: " << strerror(errno) << std::endl;
//             continue;
//         }

//         // Determine if it's a directory or file
//         if (S_ISDIR(statBuf.st_mode)) {
//             return(entry->d_name);
//         } else {
//             return(entry->d_name + " (" + std::to_string(statBuf.st_size) + " bytes)");
//         }
//     }
//     closedir(dir);
// }


void    respond_error(int error)
{
    ;
}

std::string    respond_default(void)
{
    // bool check = false;
    // search("./src_chtan/website", "main.html");
    return read_file("./src_chtan/website/error/404.html");
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

// std::string res_dir_tmp(const std::string& directory_name, const std::vector<std::string>& files)
// {    
//     std::string html = 
//             "<!DOCTYPE html>\n"
//             "<html lang=\"en\">\n"
//             "<head>\n"
//             "  <meta charset=\"UTF-8\">\n"
//             "  <title>" + directory_name + "</title>\n"
//             "  <style>\n"
//             "    body {\n"
//             "      font-family: sans-serif;\n"
//             "      margin: 1em;\n"
//             "    }\n"
//             "    h1 {\n"
//             "      font-size: 1.5em;\n"
//             "      margin-bottom: 0.2em;\n"
//             "    }\n"
//             "    hr {\n"
//             "      border: none;\n"
//             "      border-top: 1px solid #aaa;\n"
//             "      margin: 0.5em 0;\n"
//             "    }\n"
//             "    pre {\n"
//             "      font-family: monospace;\n"
//             "      line-height: 1.4;\n"
//             "    }\n"
//             "    pre a {\n"
//             "      text-decoration: none;\n"
//             "      color: #00f;\n"
//             "    }\n"
//             "    pre a:hover {\n"
//             "      text-decoration: underline;\n"
//             "    }\n"
//             "  </style>\n"
//             "</head>\n"
//             "<body>\n"
//             "  <h1>" + directory_name + "</h1>\n"
//             "  <hr>\n"
//             "  <pre>\n"
//             "<a href=\"../\">../</a>\n";

//             for (size_t i = 0; i < files.size(); ++i) {
//                 html += "<a href=\"" + files[i] + "/\">" + files[i] + "/</a>\n";
//             }
//             html += "  </pre>\n"
//             "  <hr>\n"
//             "</body>\n"
//             "</html>\n";
//         std::string head =
//             "HTTP/1.1 200 OK\r\n"
//             "Content-Type: text/html\r\n"
//             "Content-Length: " + std::to_string(html.size()) + "\r\n";
//         std::string ret = head + html;
//         return ret;
// }

// void default_templete()
// {
//     std::vector<std::string> files = read_dir("./src_chtan/website");
//     std::string tmp = res_dir_tmp("website", files);
//     std::cout << tmp << std::endl;
// }

int main(void)
{
    // response res;

    std::string tmp = respond_default();
    std::cout << tmp << std::endl;

    return 0;
}
