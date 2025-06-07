#include "../include/webserv.hpp"

Server serverA()
{
    Str root = "./website";
    CGI noCgi;

    Address listen;
    listen.push_back("127.2.2.2:9000");
    listen.push_back("127.0.199.50:8080");

    ErrorPage errorPg;
    errorPg.insert(std::make_pair(404, "/error/404.html"));
    errorPg.insert(std::make_pair(405, "/error/405.html"));

    Method m1; m1.push_back("GET");
    Route route1(
        "/",
        root,
        "",
        "main.html",
        "",
        false,
        m1,
        noCgi
    );

    Method m2; m2.push_back("GET"); m2.push_back("DELETE");
    Route route2(
        "/archive/",
        root,
        "",
        "",
        "",
        true,
        m2,
        noCgi
    );

    Method m3; m3.push_back("GET");
    Route route3(
        "/redirect/",
        root,
        "https://www.youtube.com/",
        "",
        "",
        false,
        m3,
        noCgi
    );

    Method m4; m4.push_back("GET"); m4.push_back("POST");
    Route route4(
        "/upload/",
        root,
        "",
        "upload.html",
        "/archive/",
        false,
        m4,
        noCgi
    );
    
    Method m5; m5.push_back("GET"), m5.push_back("POST"), m5.push_back("DELETE");
    CGI is_cgi; is_cgi.push_back(".py");
    Route route5(
        "/bin/",
        root,
        "",
        "showBody.py",
        "",
        false,
        m5,
        is_cgi
    );

    Method m6; m6.push_back("GET");m6.push_back("POST"); m6.push_back("DELETE");
    Route route6(
        "/test/",
        root,
        "",
        "",
        "",
        false,
        m6,
        noCgi
    );

    Method m7; m7.push_back("DELETE");
    CGI del_cgi; del_cgi.push_back(".py");
    Route route7 (
        "/delete/",
        root,
        "",
        "testDel.py",
        "",
        false,
        m7,
        del_cgi
    );

    Routes routes;
    routes.push_back(route1);
    routes.push_back(route2);
    routes.push_back(route3);
    routes.push_back(route4);
    routes.push_back(route5);
    routes.push_back(route6);
    routes.push_back(route7);

    Server s(
        "www.example.com",
        root,
        20000,
        listen,
        errorPg,
        routes
    );

    return s;
}