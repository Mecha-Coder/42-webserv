#include "../include/webserv.hpp"

bool deleteFile(const Str& filePath) 
{ 
    return std::remove(filePath.c_str()) == 0;
}

bool deleteFolder(const Str& path)
{
    DIR* dir = opendir(path.c_str());
    if (!dir) return false;

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) 
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (entry->d_type == DT_DIR)
        {
            if (!deleteFolder(path + entry->d_name + "/"))
            {
                closedir(dir);
                return false;
            }
        }
        else
        {
            if (!deleteFile(path + entry->d_name))
            {
                closedir(dir);
                return false;
            }
        }
    }
    closedir(dir);
    rmdir(path.c_str());
    return true;
}

/*
int main()
{
    if (deleteFile("/mnt/c/Users/PC/Desktop/Webserve/test/website/deleteme.txt"))
        std::cout << "Successfully deleted file" << std::endl;
    else 
        std::cout << "Failed to delete file" << std::endl;

    if (deleteFolder("/mnt/c/Users/PC/Desktop/Webserve/test/website/folder/"))
        std::cout << "Successfully folder" << std::endl;
    else
        std::cout << "Failed to delete folder" << std::endl;
}
*/