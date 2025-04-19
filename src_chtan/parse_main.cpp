#include "../inc/web.hpp"

int ft_parsing(void* buffer, int buffer_len, http_header* header)
{
    struct http_header *header_tmp = header;
    char* buffer_tmp = static_cast<char*>(buffer);
    char* method = nullptr;
    char* path = nullptr;
    char* version = nullptr;
}
