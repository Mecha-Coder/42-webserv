#include "../include/webserv.hpp"

/* 
Purpose:
- Read the raw bytes of a given file
- Return boolean if file reading is successful
- Also fills the file content into "content" passed as param

Note: Binary datatype is std::vector<char>

_______________________________________________________________________________

Other info:

Bitmask:
- in    : 0001 => Open the file for input.  
- binary: 0010 => Open the file in binary mode, not text mode.
- ate   : 0100 => Open the file and immediately seek to the end

Filesize:
- tellg => get position of the last byte
- seekg => rewind to the first byte

_______________________________________________________________________________

Return:
     0 ➡️ Successful read
    -1 ➡️ Fail
*/
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

/*
int main()
{
    Binary content;
    Str data;

    if (readFile("../website/archive/random_numbers.txt", content))
    {
        data.append(&content[0], content.size());
        std::cout << GREEN "txt file\n\n" RESET
                  << data << std::endl;
        content.clear(); data.clear();
    }

    if (readFile("../website/archive/cpp_02.jpg", content))
    {
        data.append(&content[0], content.size());
        std::cout << GREEN "image file\n\n" RESET
                  << data << std::endl;
        content.clear(); data.clear();
    }
    
    if (readFile("../website/archive/Deriv.pdf", content))
    {
        data.append(&content[0], content.size());
        std::cout << GREEN "pdf\n\n" RESET
                  << data << std::endl;
        content.clear(); data.clear();
    }
}
*/
