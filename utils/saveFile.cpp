#include "../include/webserv.hpp"

bool saveFile(const Str &filename, const Str &data)
{
    std::ofstream File(filename.c_str(), std::ios::binary); 
    if (!File) 
        return false; 
    
    File.write(&(*data.begin()), data.size());
    
    if (!File)
        return false;
    return (File.close(), true);
}