/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_dir.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chtan <chtan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 22:40:53 by chtan             #+#    #+#             */
/*   Updated: 2025/05/18 10:55:59 by chtan            ###   ########.fr       */
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
