#include <string>

typedef std::string Str; 

// GET 

/* 
1) 403 Forbidden 
- URI is a directory
- Server did not pre-define file
- autoindex turn OFF
*/ 

/* 
2) 200 OK 
- URI is a directory
- Server did not pre-define file
- autoindex turn ON
*/ 

/*
3) 404 Not Found
- URI specify file
- But file not exist
*/

/*
4) 200 OK
- URI specify file
- File can be found
- Not a cgi
*/

/*
5) CGI
- URI specify file
- File can be found
- It is a CGI
*/


Str get1 = "GET / HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:138.0) Gecko/20100101 Firefox/138.0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
            "Accept-Language: en-US,en;q=0.5\r\n"
            "Accept-Encoding: gzip, deflate, br, zstd\r\n"
            "DNT: 1\r\n"
            "Sec-GPC: 1\r\n"
            "Connection: keep-alive\r\n"
            "Upgrade-Insecure-Requests: 1\r\n"
            "Sec-Fetch-Dest: document\r\n"
            "Sec-Fetch-Mode: navigate\r\n"
            "Sec-Fetch-Site: none\r\n"
            "Sec-Fetch-User: ?1\r\n"
            "Priority: u=0, i\r\n"
            "\r\n";

Str get2 = "GET /archive HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:138.0) Gecko/20100101 Firefox/138.0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
            "Accept-Language: en-US,en;q=0.5\r\n"
            "Accept-Encoding: gzip, deflate, br, zstd\r\n"
            "DNT: 1\r\n"
            "Sec-GPC: 1\r\n"
            "Connection: close\r\n"
            "Upgrade-Insecure-Requests: 1\r\n"
            "Sec-Fetch-Dest: document\r\n"
            "Sec-Fetch-Mode: navigate\r\n"
            "Sec-Fetch-Site: none\r\n"
            "Sec-Fetch-User: ?1\r\n"
            "Priority: u=0, i\r\n"
            "\r\n";

Str get3 = "GET /awesome.html HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:138.0) Gecko/20100101 Firefox/138.0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
            "Accept-Language: en-US,en;q=0.5\r\n"
            "Accept-Encoding: gzip, deflate, br, zstd\r\n"
            "DNT: 1\r\n"
            "Sec-GPC: 1\r\n"
            "Connection: close\r\n"
            "Upgrade-Insecure-Requests: 1\r\n"
            "Sec-Fetch-Dest: document\r\n"
            "Sec-Fetch-Mode: navigate\r\n"
            "Sec-Fetch-Site: none\r\n"
            "Sec-Fetch-User: ?1\r\n"
            "Priority: u=0, i\r\n"
            "\r\n";

Str get4  = "GET /awesome.html HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:138.0) Gecko/20100101 Firefox/138.0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
            "Accept-Language: en-US,en;q=0.5\r\n"
            "Accept-Encoding: gzip, deflate, br, zstd\r\n"
            "DNT: 1\r\n"
            "Sec-GPC: 1\r\n"
            "Connection: close\r\n"
            "Upgrade-Insecure-Requests: 1\r\n"
            "Sec-Fetch-Dest: document\r\n"
            "Sec-Fetch-Mode: navigate\r\n"
            "Sec-Fetch-Site: none\r\n"
            "Sec-Fetch-User: ?1\r\n"
            "Priority: u=0, i\r\n"
            "\r\n";

Str get5  = "GET /print.cpp HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:138.0) Gecko/20100101 Firefox/138.0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
            "Accept-Language: en-US,en;q=0.5\r\n"
            "Accept-Encoding: gzip, deflate, br, zstd\r\n"
            "DNT: 1\r\n"
            "Sec-GPC: 1\r\n"
            "Connection: close\r\n"
            "Upgrade-Insecure-Requests: 1\r\n"
            "Sec-Fetch-Dest: document\r\n"
            "Sec-Fetch-Mode: navigate\r\n"
            "Sec-Fetch-Site: none\r\n"
            "Sec-Fetch-User: ?1\r\n"
            "Priority: u=0, i\r\n"
            "\r\n";

