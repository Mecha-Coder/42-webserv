#include "../include/webserv.hpp"

/////////////////////////////////////////////////////////////////////////

bool comparePathLen(const Route& a, const Route& b) 
{
    return a._path.size() > b._path.size();
}

/////////////////////////////////////////////////////////////////////////

Server server_1()
{
    List listen;
    listen.push_back("4000");
    listen.push_back("21000");
    listen.push_back("8050");

    ErrorPage errorPg;
    errorPg.insert(std::make_pair(403, "/error/403.html"));
    errorPg.insert(std::make_pair(404, "/error/404.html"));
    errorPg.insert(std::make_pair(500, "/error/500.html"));

    List noCgi;
    
    List allCgi;
        allCgi.push_back(".py");
        allCgi.push_back(".php");
        allCgi.push_back(".js");

    List onlyGET; 
        onlyGET.push_back("GET");
    
    List allMETHOD;
        allMETHOD.push_back("GET");
        allMETHOD.push_back("POST");
        allMETHOD.push_back("DELETE");

    //------------------------------------------------------------

    Route route1 (
        onlyGET,
        noCgi,
        "/",
        "",
        "dashboard.html",
        false,
        false
    );

    Route route2 (
        allMETHOD,
        noCgi,
        "/archive",
        "",
        "",
        true,
        true
    );

    Route route3 (
        onlyGET,
        noCgi,
        "/youtube",
        "https://www.youtube.com/@42berlin",
        "",
        false,
        false
    );

    Route route4 (
        onlyGET,
        noCgi,
        "/google",
        "https://maps.app.goo.gl/sqEeteD86CXqLisr6",
        "",
        false,
        false
    );

    Route route5 (
        onlyGET,
        noCgi,
        "/internal",
        "/hello.html",
        "",
        false,
        false
    );

    Route route6 (
        allMETHOD,
        allCgi,
        "/script",
        "",
        "",
        true,
        false
    );

    List onlyDEL;  onlyDEL.push_back("DELETE");
    Route route7 (
        onlyDEL,
        noCgi,
        "/dummy",
        "",
        "",
        false,
        false
    );

    List onlyPy;
    onlyPy.push_back(".py");
    Route route8 (
        allMETHOD,
        onlyPy,
        "/error",
        "",
        "",
        false,
        false
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
    std::sort(routes.begin(), routes.end(), comparePathLen);

    Server s(
        "demo.eval.com",
        listen,
        errorPg,
        routes,
        "./website/1",
        20000000
    );

    return s;
}

Server server_2()
{
    List listen;
    listen.push_back("6900");

    ErrorPage errorPg;

    List noCgi;

    List pyCgi;
        pyCgi.push_back(".py");

    List onlyGET; 
        onlyGET.push_back("GET");

    //------------------------------------------------------------

    Route route1 (
        onlyGET,
        pyCgi,
        "/",
        "",
        "main.html",
        false,
        false
    );

    Route route2 (
        onlyGET,
        noCgi,
        "/fruit",
        "",
        "fruit.html",
        false,
        false
    );

    Route route3 (
        onlyGET,
        noCgi,
        "/vegetable",
        "",
        "veggies.html",
        false,
        false
    );

    Route route4 (
        onlyGET,
        noCgi,
        "/zombie",
        "",
        "types.html",
        false,
        false
    );

    Route route5 (
        onlyGET,
        noCgi,
        "/style",
        "",
        "",
        false,
        true
    );

    Routes routes;
    routes.push_back(route1);
    routes.push_back(route2);
    routes.push_back(route3);
    routes.push_back(route4);
    routes.push_back(route5);
    std::sort(routes.begin(), routes.end(), comparePathLen);

    Server s(
        "server_2.com",
        listen,
        errorPg,
        routes,
        "./website/2",
        10
    );
    return s;
}

Server server_3()
{
    List listen;
    listen.push_back("9000");

    ErrorPage errorPg;
    errorPg.insert(std::make_pair(404, "/404.html"));

    List noCgi;
    
    List allCgi;
        allCgi.push_back(".py");
        allCgi.push_back(".php");

    List onlyGET; 
        onlyGET.push_back("GET");
    
    List allMETHOD;
        allMETHOD.push_back("GET");
        allMETHOD.push_back("POST");
        allMETHOD.push_back("DELETE");

    //------------------------------------------------------------

    Route route1 (
        onlyGET,
        noCgi,
        "/",
        "",
        "main.html",
        false,
        false
    );

    Route route2 (
        allMETHOD,
        noCgi,
        "/archive",
        "",
        "",
        true,
        true
    );

    Routes routes;
    routes.push_back(route1);
    routes.push_back(route2);
    std::sort(routes.begin(), routes.end(), comparePathLen);

    Server s(
        "server_3.com",
        listen,
        errorPg,
        routes,
        "./website/3",
        100000
    );
    return s;
}

Server server_4()
{
    List listen;
    listen.push_back("4000");

    ErrorPage errorPg;

    List noCgi;

    List onlyGET; 
        onlyGET.push_back("GET");

    //------------------------------------------------------------

    Route route1 (
        onlyGET,
        noCgi,
        "/",
        "",
        "virtual_1.html",
        false,
        false
    );

    Routes routes;
    routes.push_back(route1);

    Server s(
        "example.com",
        listen,
        errorPg,
        routes,
        "./website/virtualHost/1",
        100000
    );
    return s;
}

Server server_5()
{
    List listen;
    listen.push_back("4000");
    
    ErrorPage errorPg;

    List noCgi;

    List onlyGET; 
        onlyGET.push_back("GET");

    //------------------------------------------------------------

    Route route1 (
        onlyGET,
        noCgi,
        "/",
        "",
        "virtual_2.html",
        false,
        false
    );

    Routes routes;
    routes.push_back(route1);

    Server s(
        "test.com",
        listen,
        errorPg,
        routes,
        "./website/virtualHost/2",
        100000
    );
    return s;
}
