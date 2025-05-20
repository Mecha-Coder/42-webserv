/*
#include "../../include/Client.hpp"


const Server serverA()
{
    Address listen;
    listen.push_back("127:5.5.1:9000");
    listen.push_back("127.155.189.1:80");

    ErrorPage pages;
    pages[404] = "/error/404.html";
    pages[500] = "/error/500.html";

    Method method_1;
    method_1.push_back("GET");
    method_1.push_back("POST");

    CGI cgi_1;
    cgi_1[".php"] = "/usr/bin/php";
    cgi_1[".py"]  = "/usr/bin/python";
    
    Route route_1(
            "/",
            "./website",
            "",
            "main.html",
            "",
            false,
            method_1,
            cgi_1
        );
    
    Method method_2;
    method_2.push_back("POST");

    CGI cgi_2;
    Route route_2(
            "/upload/",
            "./website",
            "youtube.com",
            "upload.html",
            "/archive",
            false,
            method_2,
            cgi_2
        );

    Method method_3;
    method_3.push_back("GET");

    CGI cgi_3;
    Route route_3(
            "/archive/",
            "./website",
            "",
            "",
            "",
            true,
            method_3,
            cgi_3
        );
    
    Route route_4(
            "/test/",
            "/website/",
            "www.youtube.com",
            "",
            "",
            false,
            method_3,
            cgi_3
        );

    Routes routes;
    routes.push_back(route_1);
    routes.push_back(route_2);
    routes.push_back(route_3);
    routes.push_back(route_4);

    Server serverA (
        "www.Awesome.xyz",
        "./website",
        2000,
        listen,
        pages,
        routes
    );

    return serverA;
}


char get1[] = "GET /upload/done.txt HTTP/1.1\r\n"
            "Host: 127.0.0.1:8080\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:138.0) Gecko/20100101 Firefox/138.0\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*;q=0.8\r\n"
            "Accept-Language: en-US,en;q=0.5\r\n"
            "Accept-Encoding: gzip, deflate, br, zstd\r\n"
            "DNT: 1\r\n"
            "Sec-GPC: 1\r\n"
            "Upgrade-Insecure-Requests: 1\r\n"
            "Sec-Fetch-Dest: document\r\n"
            "Sec-Fetch-Mode: navigate\r\n"
            "Sec-Fetch-Site: none\r\n"
            "Sec-Fetch-User: ?1\r\n"
            "Priority: u=0, i\r\n"
            "\r\nabc123";

char get2[] = "POST /submit-form HTTP/1.1\r\nHost: 127.0.0.1:8080 \r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 28\r\nAccept: *\r\nConnection: keep-alive\r\n\r\nusername=test&password=12345";

int main()
{
    Client client(serverA());

    client.testRespond();
    std::cout << client.response() << std::endl;
}
*/
