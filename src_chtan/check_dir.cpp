/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_dir.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chtan <chtan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 22:40:53 by chtan             #+#    #+#             */
/*   Updated: 2025/05/21 12:49:40 by chtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"


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

void checkPathAndSetResponse(const std::string& path, response& res) {
    struct stat statBuf;
    if (stat(path.c_str(), &statBuf) == -1) {
        std::cerr << "Error: cannot stat path: " << strerror(errno) << std::endl;
        res.set_status("Error: cannot access path");
        return;
    }

    res.set_status(path);

    if (S_ISREG(statBuf.st_mode)) {
        res.set_content(1); // file
        res.set_content_length(std::to_string(statBuf.st_size));
    }
    else if (S_ISDIR(statBuf.st_mode)) {
        res.set_content(2); // directory
        res.set_content_length("0");

        std::string listing = listDirectoryPOSIX(path);
        std::cout << "Directory contents:\n" << listing;
        // Optionally store listing in the response object
    }
    else {
        std::cerr << "Unsupported file type." << std::endl;
        res.set_status("Unsupported file type");
    }
}

std::string* search(const std::string& path, const std::string& filename, bool &check)
{
    str* result = new std::string;
    DIR* dir = opendir(path.c_str());
    if (!dir) {
        std::cerr << "Error opening directory: " << strerror(errno) << std::endl;
        return (nullptr);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::cout << entry->d_name << std::endl;
        if (strcmp(entry->d_name, filename.c_str()) == 0) {
            closedir(dir);
            check = true;
            // return path + "/" + entry->d_name;
        }
    }

    closedir(dir);
    return (nullptr);
}

std::string search_recursive(const std::string& dir, const std::string& filename) {
    DIR* dp = opendir(dir.c_str());
    if (!dp) {
        std::cerr << "Error opening directory " << dir << ": " << strerror(errno) << std::endl;
        return "";
    }

    struct dirent* entry;
    while ((entry = readdir(dp)) != NULL) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        std::string full_path = dir + "/" + entry->d_name;

        // Check if this is a directory
        struct stat stat_buf;
        if (stat(full_path.c_str(), &stat_buf) != 0) {
            std::cerr << "Error stating " << full_path << ": " << strerror(errno) << std::endl;
            continue;
        }

        if (S_ISDIR(stat_buf.st_mode)) {
            // Recursively search subdirectory
            std::string found = search_recursive(full_path, filename);
            if (!found.empty()) {
                closedir(dp);
                return found;
            }
        } else if (S_ISREG(stat_buf.st_mode)) {
            // Check if filename matches
            if (strcmp(entry->d_name, filename.c_str()) == 0) {
                closedir(dp);
                return full_path;
            }
        }
    }

    closedir(dp);
    return "";
}

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
