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
            "",
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
    
    Routes routes;
    routes.push_back(route_1);
    routes.push_back(route_2);
    routes.push_back(route_3);

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

int main()
{
    

}