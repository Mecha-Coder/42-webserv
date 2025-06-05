#include "../include/webserv.hpp"

// #define PREFIX = "/home/jpaul/Desktop/Webserve/main"
#define PREFIX "/mnt/c/Users/PC/Desktop/Webserve/main"

Server server_4()
{
    Str root = Str(PREFIX) + "/website/4";
    CGI noCgi;

    Address listen;
    listen.push_back("127.0.0.1:9090");
    listen.push_back("127.0.0.1:8080");

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

    Method m8; m8.push_back("GET"); m8.push_back("DELETE");
    Route route8(
        "/archive/subfolder/",
        root,
        "",
        "",
        "",
        true,
        m8,
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
    routes.push_back(route8);

    Server s(
        "www.example-4.com",
        root,
        20000,
        listen,
        errorPg,
        routes
    );

    return s;
}

Server server_3()
{
    Str root = Str(PREFIX) + "/website/3";
    CGI noCgi;

    Address listen;
    listen.push_back("127.0.0.1:21000");
    listen.push_back("127.0.0.1:11000");

    ErrorPage errorPg;
    errorPg.insert(std::make_pair(404, "/404.html"));
    errorPg.insert(std::make_pair(403, "/403.html"));
    errorPg.insert(std::make_pair(500, "/500.html"));

    Method m1; m1.push_back("GET");
    Route route1(
        "/",
        root,
        "",
        "main.html",
        "/upload/",
        false,
        m1,
        noCgi
    );

    Method m2; m2.push_back("GET");
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

    CGI phpcgi; phpcgi.push_back(".php");
    Method m3; m3.push_back("GET"); m3.push_back("POST"); m3.push_back("DELETE");
    Route route3(
        "/php_cgi/",
        root,
        "",
        "",
        "",
        false,
        m3,
        phpcgi
    );

    CGI pycgi; pycgi.push_back(".py");
    Route route4(
        "/py_cgi/",
        root,
        "",
        "",
        "",
        false,
        m3,
        pycgi
    );
    
    Method m5; m5.push_back("GET");
    Route route5(
        "/style/",
        root,
        "",
        "",
        "",
        false,
        m5,
        noCgi
    );

    Method m6; m6.push_back("GET");
    Route route6(
        "/testme/",
        root,
        "",
        "test.txt",
        "",
        false,
        m6,
        noCgi
    );

    Method m7; m7.push_back("GET");
    Route route7 (
        "/upload/",
        root,
        "",
        "",
        "",
        true,
        m7,
        noCgi
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
        "www.example-3.com",
        root,
        10000,
        listen,
        errorPg,
        routes
    );

    return s;
}

Server server_2()
{
    Str root = Str(PREFIX) + "/website/2";
    CGI noCgi;

    Address listen;
    listen.push_back("127.0.0.1:9000");
    listen.push_back("127.0.0.1:8000");

    ErrorPage errorPg;

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

    Method m2; m2.push_back("GET");
    Route route2(
        "/fruits/",
        root,
        "",
        "fruits.html",
        "",
        false,
        m2,
        noCgi
    );
    
    Method m5; m5.push_back("GET");
    Route route3(
        "/script/",
        root,
        "",
        "hello.c",
        "",
        false,
        m5,
        noCgi
    );

    Method m6; m6.push_back("GET");
    Route route4(
        "/vegetables/",
        root,
        "",
        "veggies.html",
        "",
        false,
        m6,
        noCgi
    );

    Method m7; m7.push_back("GET");
    Route route5 (
        "/zombie/",
        root,
        "",
        "types.html",
        "",
        false,
        m7,
        noCgi
    );

    Routes routes;
    routes.push_back(route1);
    routes.push_back(route2);
    routes.push_back(route3);
    routes.push_back(route4);
    routes.push_back(route5);

    Server s(
        "www.testing.org.my",
        root,
        10000,
        listen,
        errorPg,
        routes
    );

    return s;
}

Server server_1()
{
    Str root = Str(PREFIX) + "/website/1";
    CGI noCgi;

    Address listen;
    listen.push_back("127.0.0.1:5000");
    listen.push_back("127.0.0.1:8060");

    ErrorPage errorPg;
    errorPg.insert(std::make_pair(404, "/404.html"));

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

    Routes routes;
    routes.push_back(route1);

    Server s(
        "www.myWebsite.gov",
        root,
        10000,
        listen,
        errorPg,
        routes
    );

    return s;
}