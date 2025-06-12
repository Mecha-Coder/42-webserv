#include "../include/webserv.hpp"

bool isDirExist(const Str dir) 
{
    struct stat info;
    
    if (stat(dir.c_str(), &info) == 0)
        return (info.st_mode & S_IFDIR);
    return false; 
}

///////////////////////////////////////////////////////////////////////

bool isFileExist(const Str path) 
{
    struct stat info;
    if (stat(path.c_str(), &info) == 0)
    {
        return (S_ISREG(info.st_mode));
    } 
    return false;
}

/////////////////////////////////////////////////////////////////////

/* OK
int main()
{
    std::cout << "Valid file" + toStr(isFileExist("../website/archive/Deriv.pdf")) << std::endl;
    std::cout << "Invalid file" + toStr(isFileExist("../website/archive/test_folder")) << std::endl;

    std::cout << "Valid folder" + toStr(isDirExist("../website/archive/test_folder")) << std::endl;
    std::cout << "Invalid folder" + toStr(isDirExist("../website/archive/Deriv.pdf")) << std::endl;
}
*/