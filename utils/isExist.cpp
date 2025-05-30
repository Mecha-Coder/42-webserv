#include "../include/webserv.hpp"

bool isFolderExist(const Str dir) 
{
    struct stat info;
    
    if (stat(dir.c_str(), &info) == 0)
        return (info.st_mode & S_IFDIR);
    return false; 
}

bool isFileExist(const Str path) 
{
    struct stat info;
    if (stat(path.c_str(), &info) == 0)
    {
        return (S_ISREG(info.st_mode) && (info.st_mode & S_IRUSR));
    } 
    return false;
}