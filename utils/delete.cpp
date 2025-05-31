#include "../include/webserv.hpp"

bool isFileExist(const Str path);

////////////////////////////////////////////////////////////////////////////////

bool deleteFile(const Str& filePath) 
{ 
    if (isFileExist(filePath))
        return (std::remove(filePath.c_str()) == 0);
    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool deleteDir(const Str& path)
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
            if (deleteDir(path + entry->d_name + "/") == false)
                return (closedir(dir), false);
        }
        else
        {
            if (deleteFile(path + entry->d_name) == false)
                return (closedir(dir), false);
        }
    }
    closedir(dir);
    return (rmdir(path.c_str()) == 0);
}

/////////////////////////////////////////////////////////////////////////////////

/* OK
int main()
{
    //std::cout << "Valid file " + toStr(deleteFile("../deleteme.txt")) << std::endl;
    //std::cout << "File is not there " + toStr(deleteFile("../deleteme.txt")) << std::endl; ; 
    //std::cout << "Invalid file " + toStr(deleteFile("../website/archive/test_folder/another_folder")) << std::endl;
    
    //std::cout << "folder with stuff " + toStr(deleteDir("../zzDeleteme/")) << std::endl;
    //std::cout << "empty folder" + toStr(deleteDir("../zzempty")) << std::endl;
    //std::cout << "Folder is not there " + toStr(deleteDir("../zzDeleteme")) << std::endl;
    //std::cout << "Invalid folder " + toStr(deleteDir("../website/archive/test_folder/cpp_02.jpg")) << std::endl;
}
*/
