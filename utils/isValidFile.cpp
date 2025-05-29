#include "../include/webserv.hpp"

/* 
Purpose:
- Using stat() function to determine path given is a valid file
_______________________________________________________________________________

Return:
    True  ➡️ Valid file
    False ➡️ Invalid file return 404
*/
bool isFileExist(const Str path) 
{
    struct stat info;
    if (stat(path.c_str(), &info) == 0)
    {
        return (S_ISREG(info.st_mode) && (info.st_mode & S_IRUSR));
    } 
    return false;
}

/*
int main()
{
    Str root = "/mnt/c/Users/PC/Desktop/Webserve/test/website";
    Str File1 = "/bin/removeME.py";
    Str File2 = "/archive/cpp_02.jpg";
    Str File3 = "/error/404.html";
    Str File4 = "/error/500.html";

    if (isValidFile(root + File1))
        std::cout << root + File1 << " exists." << std::endl;
    else
        std::cout << "Not exist" << std::endl;
    
    if (isValidFile(root + File2))
        std::cout << root + File2 << " exists." << std::endl;
    else
        std::cout << "Not exist" << std::endl;
    
    if (isValidFile(root + File3))
        std::cout << root + File3 << " exists." << std::endl;
    else
        std::cout << "Not exist" << std::endl;
    
    if (isValidFile(root + File4))
        std::cout << root + File4 << " exists." << std::endl;
    else
        std::cout << "Not exist" << std::endl;
}
*/
