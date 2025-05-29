#include "../include/webserv.hpp"

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