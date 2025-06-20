#include "../include/webserv.hpp"

bool readFile(const Str& filename, Str &content) 
{
    std::ifstream File(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
    if (!File)
        return false;

    std::ifstream::pos_type size = File.tellg();
    if (size == std::ifstream::pos_type(-1))
        return false;

    if (size == 0)
        return (true);

    content.resize(static_cast<std::size_t>(size));
    File.seekg(0, std::ios::beg);
    File.read(&content[0], static_cast<std::streamsize>(size));

    return File.good();
}

//////////////////////////////////////////////////////////////////////////////////////////

bool readDir(const Str &path, DirItems &items)
{
    DIR* dir = opendir(path.c_str());
    if (!dir) 
		return false;

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) 
    {
        /* Skip ".", "..", symbolic link, and unknown */
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 
            || entry->d_type == DT_LNK || entry->d_type == DT_UNKNOWN)
            continue;

        items[entry->d_name] =  (entry->d_type == DT_DIR);
    }
    closedir(dir);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////

/* OK
void test_readFile(const Str &filepath)
{
    Str content;
    if (readFile(filepath, content))
        std::cout << "Here is the content\n======\n [" + content + "]\n" << std::endl;
    else
        std::cout << "Fail to read file\n" << std::endl; 
}

void test_readDir(const Str &Path)
{
    DirItems items;
    if (readDir(Path, items))
    {
        std::cout << "Dir list\n=====" << std::endl;
        DirItems::const_iterator i = items.begin();

        for(; i != items.end() ; i++)
        {  std::cout << i->first << std::endl;  } 
        std::cout << "\n" << std::endl;
    }
    else
        std::cout << "Fail to read folder\n" << std::endl; 
}

int main()
{
    //test_readFile("../website/archive/test_folder/random_numbers.txt"); // Text file
    //test_readFile("../website/archive/test_folder/random_numbers.tx"); // Invalid filepath
    //test_readFile("../website/archive/test_folder/blank.txt"); // Blank file
    //test_readFile("../website/archive/test_folder/test_image.jpeg"); // Image file

    //test_readDir("../website/archive/test_folder/another_folder"); // Empty folder
    //test_readDir("../website/archive/test_folder/random_numbers.tx"); // Invalid folder
    //test_readDir("../website/archive"); // with files and nested subfolders
}
*/