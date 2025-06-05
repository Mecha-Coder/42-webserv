#include "../include/webserv.hpp"

Server server_1()
{
    Address listen;
    listen.push_back("127.0.0.1:4000");
    listen.push_back("127.0.0.1:21000");
    listen.push_back("127.0.0.1:8050");

    ErrorPage errorPg;
    errorPg.insert(std::make_pair(404, "/error/404.html"));
    errorPg.insert(std::make_pair(405, "/error/405.html"));
    errorPg.insert(std::make_pair(500, "/error/500.html"));

    Str mainRoot = "./website/1";
    CGI noCgi;
    
    CGI allCgi;
        allCgi.push_back(".py");
        allCgi.push_back(".php");

    Method onlyGET; 
        onlyGET.push_back("GET");
    
    Method allMETHOD;
        allMETHOD.push_back("GET");
        allMETHOD.push_back("POST");
        allMETHOD.push_back("DELETE");

    //------------------------------------------------------------

    Route route1 (
        "/",
        mainRoot,
        "",
        "dashboard.html",
        "",
        false,
        onlyGET,
        noCgi
    );

    Route route2 (
        "/archive/",
        mainRoot,
        "",
        "",
        "/archive/",
        true,
        allMETHOD,
        noCgi
    );

    Route route3 (
        "/archive/subfolder/",
        mainRoot,
        "",
        "",
        "",
        true,
        onlyGET,
        noCgi
    );

    Route route4 (
        "/youtube/",
        mainRoot,
        "https://www.youtube.com/@42berlin",
        "",
        "",
        false,
        onlyGET,
        noCgi
    );

    Route route5 (
        "/google/",
        mainRoot,
        "https://maps.app.goo.gl/sqEeteD86CXqLisr6",
        "",
        "",
        false,
        onlyGET,
        noCgi
    );
    
    Route route6 (
        "/internal/",
        mainRoot,
        "/hello.html",
        "",
        "",
        false,
        onlyGET,
        noCgi
    );

    Route route7 (
        "/script/",
        mainRoot,
        "",
        "",
        "",
        true,
        allMETHOD,
        noCgi
    );

    Route route8 (
        "/photos/",
        mainRoot,
        "",
        "",
        "",
        false,
        onlyGET,
        noCgi
    );

    Route route9 (
        "/error/",
        mainRoot,
        "",
        "",
        "",
        false,
        onlyGET,
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
    routes.push_back(route8);
    routes.push_back(route9);

    Server s(
        "www.demoEvalSite.com",
        mainRoot,
        20000000,
        listen,
        errorPg,
        routes
    );

    return s;
}

Server server_2()
{
    Address listen;
    listen.push_back("127.0.0.1:6900");

    ErrorPage errorPg;

    Str mainRoot = "./website/2";
    CGI noCgi;

    CGI onlyPython;
        onlyPython.push_back(".py");
    
    CGI allCgi;
        allCgi.push_back(".py");
        allCgi.push_back(".php");

    Method onlyGET; 
        onlyGET.push_back("GET");
    
    Method allMETHOD;
        allMETHOD.push_back("GET");
        allMETHOD.push_back("POST");

    //------------------------------------------------------------

    Route route1 (
        "/",
        mainRoot,
        "",
        "main.html",
        "",
        false,
        allMETHOD,
        onlyPython
    );

    Route route2 (
        "/fruit/",
        mainRoot,
        "",
        "fruit.html",
        "",
        false,
        onlyGET,
        noCgi
    );

    Route route3 (
        "/vegetable/",
        mainRoot,
        "",
        "veggies.html",
        "",
        false,
        onlyGET,
        noCgi
    );

    Route route4 (
        "/zombie/",
        mainRoot,
        "",
        "types.html",
        "",
        false,
        onlyGET,
        noCgi
    );

    Route route5 (
        "/style/",
        mainRoot,
        "",
        "",
        "",
        true,
        onlyGET,
        noCgi
    );

    Method onlyPOST; onlyPOST.push_back("POST");
    Route route6 (
        "/upload/",
        mainRoot,
        "",
        "",
        "",
        false,
        onlyPOST,
        noCgi
    );

    Routes routes;
    routes.push_back(route1);
    routes.push_back(route2);
    routes.push_back(route3);
    routes.push_back(route4);
    routes.push_back(route5);
    routes.push_back(route6);

    Server s(
        "www.server_2.com.my",
        mainRoot,
        10,
        listen,
        errorPg,
        routes
    );
    return s;
}

Server server_3()
{
    Address listen;
    listen.push_back("127.0.0.1:9000");

    ErrorPage errorPg;
    errorPg.insert(std::make_pair(404, "/404.html"));

    Str mainRoot = "./website/3";
    CGI noCgi;
    
    CGI allCgi;
        allCgi.push_back(".py");
        allCgi.push_back(".php");

    Method onlyGET; 
        onlyGET.push_back("GET");
    
    Method allMETHOD;
        allMETHOD.push_back("GET");
        allMETHOD.push_back("POST");
        allMETHOD.push_back("DELETE");

    //------------------------------------------------------------

    Route route1 (
        "/",
        mainRoot,
        "",
        "main.html",
        "",
        false,
        onlyGET,
        noCgi
    );

    Route route2 (
        "/archive/",
        mainRoot,
        "",
        "",
        "/archive/",
        true,
        allMETHOD,
        noCgi
    );

    Routes routes;
    routes.push_back(route1);
    routes.push_back(route2);

    Server s(
        "www.server_3.com.my",
        mainRoot,
        10000,
        listen,
        errorPg,
        routes
    );
    return s;
}
