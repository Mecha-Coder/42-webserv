#include "../include/webserv.hpp"

bool readFile(const Str& filename, Str &content) 
{
    std::ifstream File(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
    
    if (!File) return false;

    std::ifstream::pos_type size = File.tellg();
    if (size == std::ifstream::pos_type(-1))  return false;

    if (size == 0) return (true);

    content.resize(static_cast<std::size_t>(size));
    File.seekg(0, std::ios::beg);
    File.read(&content[0], static_cast<std::streamsize>(size));

    return File.good();
}

bool readDir(const Str &path, DirItems &items)
{
    DIR* dir = opendir(path.c_str());
    if (!dir) 
		return false;

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) 
    {
        /* Skip (".")  ("..") (symbolic link) and (unknown) */
        if (strcmp(entry->d_name, ".") == 0 
			|| strcmp(entry->d_name, "..") == 0 
            || entry->d_type == DT_LNK 
			|| entry->d_type == DT_UNKNOWN)
            continue;

        items[entry->d_name] =  (entry->d_type == DT_DIR);
    }
    closedir(dir);
    return true;
}