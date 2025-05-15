#include "include/Client.hpp"

#define YELLOW "\033[33m"
#define RESET  "\033[0m"

Server serverA(){}
void processReq(Client &client){}

Str Missing_Path_and_Version() 
{
std::cout << YELLOW "Missing_Path_and_Version\n"
          << "========================" RESET << std::endl;

Str req =   "PATCH \r\n"
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
return req;
}

Str Missing_Version() 
{
std::cout << YELLOW "Missing_Version\n"
          << "========================" RESET << std::endl;

Str req =   "GET /test/ \r\n"
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
return req;
}

Str Invalid_Version() 
{
std::cout << YELLOW "Invalid_Version\n"
          << "========================" RESET << std::endl;

Str req  = "GET /test HTTP/2.1\r\n"
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
return req;
}

Str Post_No_ContentLen() 
{
std::cout << YELLOW "Post_No_ContentLen\n"
          << "========================" RESET << std::endl;

Str req  =  "POST /submit-form HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080 \r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "Accept: */*\r\n"
            "Connection: keep-alive\r\n"
            "\r\n"
            "username=test&password=12345";
return req;
}

Str Post_No_ContentType() 
{
std::cout << YELLOW "Post_No_ContentType\n"
          << "========================" RESET << std::endl;

Str req  =  "POST /submit-form HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080 \r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
            "Content-Length: 28\r\n"
            "Accept: */*\r\n"
            "Connection: keep-alive\r\n"
            "\r\n"
            "username=test&password=12345";
return req;
}

Str Bodysize_Not_Same_ContentLen() 
{
std::cout << YELLOW "Bodysize_Not_Same_ContentLen\n"
          << "========================" RESET << std::endl;

Str req  =  "POST /submit-form HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080 \r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "Content-Length: 200\r\n"
            "Accept: */*\r\n"
            "Connection: keep-alive\r\n"
            "\r\n"
            "username=test&password=12345";
return req;
}

Str Body_Exceed_Limit() 
{
std::cout << YELLOW "Body_Exceed_Limit\n"
          << "========================" RESET << std::endl;

Str req  =  "POST /submit-form HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080 \r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "Content-Length: 573\r\n"
            "Accept: */*\r\n"
            "Connection: keep-alive\r\n"
            "\r\n"
            "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum";
return req;
}

Str Redirect_Consistent_Format() 
{
std::cout << YELLOW "Redirect_Consistent_Format\n"
          << "==================================" RESET << std::endl;

Str req  =  "POST /submit-form HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080 \r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "Content-Length: 28\r\n"
            "Accept: */*\r\n"
            "Connection: keep-alive\r\n"
            "\r\n"
            "username=test&password=12345";
return req;
}

Str Invalid_Route() 
{
std::cout << YELLOW "Invalid_Route\n"
          << "========================" RESET << std::endl;

Str req  =  "POST /submit-form/what.html HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080 \r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "Content-Length: 28\r\n"
            "Accept: */*\r\n"
            "Connection: keep-alive\r\n"
            "\r\n"
            "username=test&password=12345";
return req;
}

Str Redirect() 
{
std::cout << YELLOW "Redirect_please\n"
          << "========================" RESET << std::endl;

Str req =   "GET /redirect/ HTTP/1.1\r\n"
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
return req;
}

Str Forbidden_Method()
{
std::cout << YELLOW "Forbidden_Method\n"
          << "========================" RESET << std::endl;

Str req =   "PATCH / HTTP/1.1\r\n"
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
return req;
}

Str Method_Not_Listed_Route()
{
std::cout << YELLOW "Method_Not_Listed_Route()\n"
          << "========================" RESET << std::endl;

Str req =   "GET /upload/ HTTP/1.1\r\n"
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
return req;
}

Str No_Such_File()
{
std::cout << YELLOW "No_Such_file\n"
          << "======================" RESET << std::endl;

Str req =   "GET /unknown.txt \r\n"
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
return req;
}

Str No_Default_no_autoindex()
{
std::cout << YELLOW "No_Default_no_autoindex\n"
          << "======================" RESET << std::endl;

Str req =   "GET /cgi-bin/ \r\n"
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
return req;
}

Str No_Default_autoindex_ON()
{
std::cout << YELLOW "No_Default_autoindex_ON\n"
          << "======================" RESET << std::endl;

Str req =   "GET /archive/ \r\n"
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
return req;
}

Str Fetch_default_file()
{
std::cout << YELLOW "Fetch_default_file\n"
          << "======================" RESET << std::endl;

Str req =   "GET / \r\n"
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
return req;
}

Str Fetch_large_image()
{
    std::cout << YELLOW "Fetch_large_image\n"
          << "======================" RESET << std::endl;

Str req =   "GET /archive/cpp_02.jpg \r\n"
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
return req;
}

void test(const Str &request)
{
    Client client(serverA());

    client.appendReq(request);
    processReq(client);
    std::cout << client.respond() << std::endl;
}

int main()
{
    // 1) Validation 
        // 1.a) Malform request (400 Bad Request)
            test(Missing_Path_and_Version());
            test(Missing_Version());
            test(Invalid_Version());
    
        // 1.b) POST request: Body size issue (400 Bad Request)
            test(Post_No_ContentLen());
            test(Post_No_ContentType());
            test(Bodysize_Not_Same_ContentLen());
        
        // 1.c) Post request: Body exceed limit in config (413 Payload Too Large)
            test(Body_Exceed_Limit());
        
    // 2) Check any route match URI & meet route requirement

        // 2.a) send /upload -> redirect to /upload/ to make it consistent
        // (308 Permanent Redirect)
            test(Redirect_Consistent_Format());

        // 2.b) No route match URI (404 Not Found)
            test(Invalid_Route());
    
        // 2.c) Route is a redirect (301 Moved Permanently)
            test(Redirect());
        
        // 2.d) (405 Method Not Allowed)
            test(Forbidden_Method());
            test(Method_Not_Listed_Route());

    // 3) GET Request
        // 3.a) URI specify file but not exist (404 Not Found)
            test(No_Such_File());
        
        // 3.b) No default file, autoindex off (403 Forbidden)
            test(No_Default_no_autoindex());

        // 3.c) Show me your autoindex (200 OK)
            test(No_Default_autoindex_ON());
        
        // 3.d) Show me your file Fetching (200 OK)
            test(Fetch_default_file());
            test(Fetch_large_image());
}       