/*
#include "../../include/Server.hpp"

void test_find_route(Server &server, const Str &uri)
{
    const Route *route = server.findRoute(uri);

    if (route) 
    {
        std::cout << "Route " << uri << " is available" << std::endl;
        route->showData();
    }
    else    std::cout << "Route " << uri << " is not available" << std::endl;
}

void test_isMyAddr(Server &server, const Str &addr)
{
    if (server.isMyAddr(addr))
        std::cout << "Address: " << addr << " belongs to this server" << std::endl;
    else
        std::cout << "Address: " << addr << " does not belong to this server" << std::endl;
}

void test_myErrorPage(Server &server, const int &code)
{
    const Str path = server.myErrorPg(code);

    if (!path.empty()) std::cout << "Custom error page for code " << code << " = " << path << std::endl;
    else                std::cout << "No page for " << code << ". Please use default error page" << std::endl; 
}

int main()
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

    serverA.showData();
    std::cout << "-------------------------------" << std::endl;
    test_find_route(serverA, "/");
    test_find_route(serverA, "/upload/");
    test_find_route(serverA, "/archive/");
    test_find_route(serverA, "//");
    test_find_route(serverA, "/section/");

    std::cout << "-------------------------------" << std::endl;
    test_isMyAddr(serverA, "127:5.5.1:9000");
    test_isMyAddr(serverA, "127.155.189.1:80");
    test_isMyAddr(serverA, "127.5.5.1:8080");
    std::cout << "-------------------------------" << std::endl;

    test_myErrorPage(serverA, 405);
    test_myErrorPage(serverA, 404);
    test_myErrorPage(serverA, 500);
    std::cout << "-------------------------------" << std::endl;

    IP_Host list;
    serverA.giveMyAddr(list);
    std::cout << "Listening address for " << serverA._serverName << std::endl;
    for (IP_Host::const_iterator i = list.begin(); i != list.end(); i++)
        std::cout << "\t" << i->first << ":" << i->second << std::endl;
    std::cout << std::endl;
}
*/