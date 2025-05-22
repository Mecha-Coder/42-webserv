#include "../include/webserv.hpp"

/* 
Purpose:
- Using stat() function to determine path given is a valid folder/direcrtory
_______________________________________________________________________________

Return:
    True  ➡️ Valid folder
    False ➡️ Invalid folder/route return 404
*/
bool isFolderExist(const Str dir) 
{
    struct stat info;
    
    if (stat(dir.c_str(), &info) == 0)
        return (info.st_mode & S_IFDIR);
    return false; 
}

/*
int main()
{
    Str root = "/home/jpaul/Desktop/Webserve/test/website";
    Str URI_1 = "/archive";
    Str URI_2 = "/unknown";
    Str URI_3 = "/bin";

    if (isFolderExist(root + URI_1))
        std::cout << root + URI_1 << " exists." << std::endl;
    else
        std::cout << "Directory does NOT exist." << std::endl;
    
    if (isFolderExist(root + URI_2))
        std::cout << root + URI_2 << " exists." << std::endl;
    else
        std::cout << "Directory does NOT exist." << std::endl;
    
    if (isFolderExist(root + URI_3))
        std::cout << root + URI_3 << " exists." << std::endl;
    else
        std::cout << "Directory does NOT exist." << std::endl;
}
*/
