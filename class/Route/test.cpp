/*
#include "../../include/Route.hpp"

void test_cgi_path(Route &route_1, const Str &file)
{
    const Str cgi_path = route_1.runWithCGI(file);

    if (!cgi_path.empty()) 
        std::cout << "Yes, file " << file << " handled by CGI. Path >  " << cgi_path << std::endl;
    else
        std::cout << "File " << file << " not handled by CGI" << std::endl;
}

void test_allowed_method(Route &route_1, const Str &method)
{
    if (route_1.isMethodAllow(method))
        std::cout << "Method: " << method << " is allowed" << std::endl;
    else
        std::cout << "Method: " << method << " is forbidden to use" << std::endl;
}

int main()
{
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

    route_1.showData();
    test_cgi_path(route_1, "print.c");
    test_cgi_path(route_1, route_1._defaultFile);
    test_cgi_path(route_1, "explode.php");
    test_cgi_path(route_1, "game.py");

    std::cout << std::endl;
    test_allowed_method(route_1, "GET");
    test_allowed_method(route_1, "POST");
    test_allowed_method(route_1, "DELETE");
}
*/
