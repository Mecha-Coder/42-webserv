#include "../include/webserv.hpp"

void test(const Str &request)
{
    Client client(serverA());

    client.appendReq(request);
    std::cout << GREEN "\nIN\n===" RESET << std::endl; showRawStr(request);
    std::cout << CYAN "OUT\n===" RESET << std::endl;
    processReq(client);
    showRawStr(client.respond());
}

Str Missing_Path_and_Version() 
{
std::cout << YELLOW "\n=================================================================\n"
          << "1.a) Missing_Path_and_Version\n" 
          << "=================================================================" RESET << std::endl;

Str req =   "PATCH \r\n"
            "Host: 127.0.0.1:8080\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:138.0) Gecko/20100101 Firefox/138.0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*;q=0.8\r\n"
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
std::cout << YELLOW "\n=================================================================\n"
          << "1.a) Missing_Version\n" 
          << "=================================================================" RESET << std::endl;

Str req =   "GET /test/ \r\n"
            "Host: 127.0.0.1:8080\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:138.0) Gecko/20100101 Firefox/138.0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*;q=0.8\r\n"
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
std::cout << YELLOW "\n=================================================================\n"
          << "1.a) Invalid_Version\n" 
          << "=================================================================" RESET << std::endl;

Str req  = "GET /test HTTP/2.1\r\n"
            "Host: 127.0.0.1:8080\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:138.0) Gecko/20100101 Firefox/138.0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*;q=0.8\r\n"
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
std::cout << YELLOW "\n=================================================================\n"
          << "1.b) Post_No_ContentLen\n" 
          << "=================================================================" RESET << std::endl;

Str req  =  "POST /submit-form HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080 \r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "Accept: *\r\n"
            "Connection: keep-alive\r\n"
            "\r\n"
            "username=test&password=12345";
return req;
}

Str Post_No_ContentType() 
{
std::cout << YELLOW "\n=================================================================\n"
          << "1.b) Post_No_ContentType\n" 
          << "=================================================================" RESET << std::endl;

Str req  =  "POST /submit-form HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080 \r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
            "Content-Length: 28\r\n"
            "Accept: *\r\n"
            "Connection: keep-alive\r\n"
            "\r\n"
            "username=test&password=12345";
return req;
}

Str Bodysize_Not_Same_ContentLen() 
{
std::cout << YELLOW "\n=================================================================\n"
          << "1.b) Bodysize_Not_Same_ContentLen\n" 
          << "=================================================================" RESET << std::endl;

Str req  =  "POST /submit-form HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080 \r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "Content-Length: 200\r\n"
            "Accept: *\r\n"
            "Connection: keep-alive\r\n"
            "\r\n"
            "username=test&password=12345";
return req;
}

Str Body_Exceed_Limit() 
{
std::cout << YELLOW "\n=================================================================\n"
          << "1.c) Body_Exceed_Limit\n" 
          << "=================================================================" RESET << std::endl;

Str req  =  "POST /submit-form HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080 \r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "Content-Length: 573\r\n"
            "Accept: *\r\n"
            "Connection: keep-alive\r\n"
            "\r\n"
            "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum";
return req;
}

Str Redirect_Consistent_Format() 
{
std::cout << YELLOW "\n=================================================================\n"
          << "2.a) Redirect_Consistent_Format\n" 
          << "=================================================================" RESET << std::endl;

Str req  =  "POST /submit-form HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080 \r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "Content-Length: 28\r\n"
            "Accept: *\r\n"
            "Connection: keep-alive\r\n"
            "\r\n"
            "username=test&password=12345";
return req;
}

Str Invalid_Route() 
{
std::cout << YELLOW "\n=================================================================\n"
          << "2.b) Invalid_Route\n" 
          << "=================================================================" RESET << std::endl;

Str req  =  "POST /submit-form/what.html HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080 \r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "Content-Length: 28\r\n"
            "Accept: *\r\n"
            "Connection: keep-alive\r\n"
            "\r\n"
            "username=test&password=12345";
return req;
}

Str Redirect() 
{
std::cout << YELLOW "\n=================================================================\n"
          << "2.c) Redirect\n" 
          << "=================================================================" RESET << std::endl;

Str req =   "GET /redirect/ HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:138.0) Gecko/20100101 Firefox/138.0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*;q=0.8\r\n"
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
std::cout << YELLOW "\n=================================================================\n"
          << "2.d) Forbidden_Method\n" 
          << "=================================================================" RESET << std::endl;

Str req =   "PATCH / HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:138.0) Gecko/20100101 Firefox/138.0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*;q=0.8\r\n"
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
std::cout << YELLOW "\n=================================================================\n"
          << "2.d) Method_Not_Listed_Route\n" 
          << "=================================================================" RESET << std::endl;

Str req =   "DELETE /upload/ HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:138.0) Gecko/20100101 Firefox/138.0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*;q=0.8\r\n"
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
std::cout << YELLOW "\n=================================================================\n"
          << "3.a) No_Such_File\n" 
          << "=================================================================" RESET << std::endl;

Str req =   "GET /unknown.txt HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:138.0) Gecko/20100101 Firefox/138.0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*;q=0.8\r\n"
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
std::cout << YELLOW "\n=================================================================\n"
          << "3.b) No_Default_no_autoindex\n" 
          << "=================================================================" RESET << std::endl;

Str req =   "GET /test/ HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:138.0) Gecko/20100101 Firefox/138.0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*;q=0.8\r\n"
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
std::cout << YELLOW "\n=================================================================\n"
          << "3.c) No_Default_autoindex_ON\n" 
          << "=================================================================" RESET << std::endl;

Str req =   "GET /archive/ HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:138.0) Gecko/20100101 Firefox/138.0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*;q=0.8\r\n"
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
std::cout << YELLOW "\n=================================================================\n"
          << "3.d) Fetch_default_file\n" 
          << "=================================================================" RESET << std::endl;

Str req =   "GET / HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:138.0) Gecko/20100101 Firefox/138.0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*;q=0.8\r\n"
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
std::cout << YELLOW "\n=================================================================\n"
          << "3.d) Fetch_large_image\n" 
          << "=================================================================" RESET << std::endl;

Str req =   "GET /archive/cpp_02.jpg HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:138.0) Gecko/20100101 Firefox/138.0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*;q=0.8\r\n"
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

Str Fetch_another_file()
{
std::cout << YELLOW "\n=================================================================\n"
          << "3.d) Fetch_another_file\n" 
          << "=================================================================" RESET << std::endl;

Str req =   "GET /archive/random_numbers.txt HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:138.0) Gecko/20100101 Firefox/138.0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*;q=0.8\r\n"
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

Str Simple_get_CGI()
{
std::cout << YELLOW "\n=================================================================\n"
          << "3.e) Simple_get_CGI\n" 
          << "=================================================================" RESET << std::endl;

Str req =   "GET /bin/printenv.py HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:138.0) Gecko/20100101 Firefox/138.0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*;q=0.8\r\n"
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

Str Upload_2_files() 
{
std::cout << YELLOW "\n=================================================================\n"
          << "4.a) Upload_2_files\n" 
          << "=================================================================" RESET << std::endl;

    Binary file1; readFile("./website/dummy/mime.txt", file1);
    Binary file2; readFile("./website/dummy/test_image.jpeg", file2);

Str req  =  "POST /upload/ HTTP/1.1\r\n"
            "Host: localhost:8080\r\n"
            "Connection: keep-alive\r\n"
            "Content-Length: 12496\r\n"
            "sec-ch-ua-platform: \"Windows\"\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/136.0.0.0 Safari/537.36 Edg/136.0.0.0\r\n"
            "sec-ch-ua: \"Chromium\";v=\"136\", \"Microsoft Edge\";v=\"136\", \"Not.A/Brand\";v=\"99\"\r\n"
            "DNT: 1\r\n"
            "Content-Type: multipart/form-data; boundary=----WebKitFormBoundarypfFLeGoyUU5ieF18\r\n"
            "sec-ch-ua-mobile: \?0\r\n"
            "Accept: */*\r\n"
            "Origin: http://localhost:8080\r\n"
            "Sec-Fetch-Site: same-origin\r\n"
            "Sec-Fetch-Mode: cors\r\n"
            "Sec-Fetch-Dest: empty\r\n"
            "Referer: http://localhost:8080/upload.html\r\n"
            "Accept-Encoding: gzip, deflate, br, zstd\r\n"
            "Accept-Language: en-US,en;q=0.9\r\n"
            "\r\n"
            "------WebKitFormBoundarypfFLeGoyUU5ieF18\r\n"
            "Content-Disposition: form-data; name=\"files\"; filename=\"mime.txt\"\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n";

Str req1  = "\r\n------WebKitFormBoundarypfFLeGoyUU5ieF18\r\n"
            "Content-Disposition: form-data; name=\"files\"; filename=\"test_image.jpeg\"\r\n"
            "Content-Type: image/jpeg\r\n"
            "\r\n";

Str req2  = "\r\n------WebKitFormBoundarypfFLeGoyUU5ieF18--\r\n";

    req.append(file1.begin(), file1.end());
    req += req1;
    req.append(file2.begin(), file2.end());
    req += req2;
return req;
}

Str Post_Ghost_File() 
{
std::cout << YELLOW "\n=================================================================\n"
          << "4.b) Post_Ghost_File\n" 
          << "=================================================================" RESET << std::endl;

Str req  =  "POST /bin/ghost.php HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080 \r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "Content-Length: 573\r\n"
            "Accept: *\r\n"
            "Connection: keep-alive\r\n"
            "\r\n"
            "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum";
return req;
}

Str Post_No_Default() 
{
std::cout << YELLOW "\n=================================================================\n"
          << "4.c) Post_No_Default\n" 
          << "=================================================================" RESET << std::endl;

Str req  =  "POST /test/ HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080 \r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "Content-Length: 573\r\n"
            "Accept: *\r\n"
            "Connection: keep-alive\r\n"
            "\r\n"
            "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum";
return req;
}

Str Post_Not_CGI() 
{
std::cout << YELLOW "\n=================================================================\n"
          << "4.d) Post_Not_CGI\n" 
          << "=================================================================" RESET << std::endl;

Str req  =  "POST /bin/test.php HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080 \r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "Content-Length: 573\r\n"
            "Accept: *\r\n"
            "Connection: keep-alive\r\n"
            "\r\n"
            "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum";
return req;
}

Str CGI_Post_show_body_default_file() 
{
std::cout << YELLOW "\n=================================================================\n"
          << "4.e) CGI_Post_show_body_default_file\n" 
          << "=================================================================" RESET << std::endl;

Str req  =  "POST /bin/ HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080 \r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "Content-Length: 573\r\n"
            "Accept: *\r\n"
            "Connection: keep-alive\r\n"
            "\r\n"
            "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum";
return req;
}

Str Delete_File_Not_there()
{
std::cout << YELLOW "\n=================================================================\n"
          << "5.a) Delete_File_Not_there\n" 
          << "=================================================================" RESET << std::endl;

Str req =   "DELETE /archive/cat.png HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:138.0) Gecko/20100101 Firefox/138.0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*;q=0.8\r\n"
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

Str Delete_with_CGI() 
{
std::cout << YELLOW "\n=================================================================\n"
          << "5.b) Delete_with_CGI\n" 
          << "=================================================================" RESET << std::endl;

Str req  =  "DELETE /bin/removeMe.py HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080 \r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "Content-Length: 573\r\n"
            "Accept: *\r\n"
            "Connection: keep-alive\r\n"
            "\r\n";
return req;
}

Str Delete_actual_file() 
{
std::cout << YELLOW "\n=================================================================\n"
          << "5.c) Delete_actual_file\n" 
          << "=================================================================" RESET << std::endl;

Str req  =  "DELETE /archive/flowchar.png " 
            "HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080 \r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "Content-Length: 573\r\n"
            "Accept: *\r\n"
            "Connection: keep-alive\r\n"
            "\r\n";
return req;
}

Str Delete_actual_directory() 
{
std::cout << YELLOW "\n=================================================================\n"
          << "5.d) Delete_actual_directory\n" 
          << "=================================================================" RESET << std::endl;

Str req  =  "DELETE /test/ HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080 \r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "Content-Length: 573\r\n"
            "Accept: *\r\n"
            "Connection: keep-alive\r\n"
            "\r\n";
return req;
}

Str Delete_default_cgi() 
{
std::cout << YELLOW "\n=================================================================\n"
          << "5.e) Delete_default_cgi\n" 
          << "=================================================================" RESET << std::endl;

Str req  =  "DELETE /delete/ HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080 \r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "Content-Length: 573\r\n"
            "Accept: *\r\n"
            "Connection: keep-alive\r\n"
            "\r\n";
return req;
}

int main()
{
    Server dummy(serverA());
    
    test(Upload_2_files());
    
    /*
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
            test(Fetch_another_file());
            //test(Fetch_large_image());
        
        // 3.e) Run simple cgi
            test(Simple_get_CGI());
    
   
    // 4) POST Request
        // 4.a) Path support upload
            test(Upload_2_files());
        
        // 4.b) No upload support: file doesn't exist
            test(Post_Ghost_File());

        // 4.c) No upload support: no default file
            test(Post_No_Default());

        // 4.d) No upload support: not a cgi
            test(Post_Not_CGI());

        // 4.e) No upload support: is a cgi
            test(CGI_Post_show_body_default_file());
        
    // 5) DELETE Request
        // 5.a) Delete file is not there
            test(Delete_File_Not_there());
        
        // 5.b) Delete run as CGI
            test(Delete_with_CGI());

        // 5.c) Delete actual file
            test(Delete_actual_file());
        
        // 5.d) Delete actual directory
            test(Delete_actual_directory());
            
        // 5.e) Delete URI default file & run cgi
            test(Delete_default_cgi());
        */
}
