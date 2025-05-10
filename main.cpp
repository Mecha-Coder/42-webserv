#include <string>

typedef std::string Str; 

Str SaveFile = "POST /upload HTTP/1.1\r\n"
                "Host: example.com\r\n"
                "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:125.0) Gecko/20100101 Firefox/125.0\r\n"
                "Content-Type: multipart/form-data; boundary=---------------------------12345678901234567890123456789\r\n"
                "Content-Length: 342\r\n"
                "Connection: keep-alive\r\n"
                "\r\n"
                "-----------------------------12345678901234567890123456789\r\n"
                "Content-Disposition: form-data; name=\"text_field\"\r\n"
                "Content-Type: text/plain\r\n"
                "\r\n"
                "This is sample text content in the first part of the multipart body.\r\n"
                "-----------------------------12345678901234567890123456789\r\n"
                "Content-Disposition: form-data; name=\"file\"; filename=\"example.txt\"\r\n"
                "Content-Type: text/plain\r\n"
                "\r\n"
                "This is the content of the text file being uploaded."
                "Second line of file content.\r\n"
                "-----------------------------12345678901234567890123456789--\r\n";

Str FetchFile = "GET /sample.html HTTP/1.1\r\n"
                "Host: example.com\r\n"
                "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:125.0) Gecko/20100101 Firefox/125.0\r\n"
                "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\r\n"
                "Accept-Language: en-US,en;q=0.5\r\n"
                "Accept-Encoding: gzip, deflate, br\r\n"
                "Connection: keep-alive\r\n"
                "Upgrade-Insecure-Requests: 1\r\n"
                "Sec-Fetch-Dest: document\r\n"
                "Sec-Fetch-Mode: navigate\r\n"
                "Sec-Fetch-Site: none\r\n"
                "Sec-Fetch-User: ?1\r\n"
                "\r\n";

