#include "../include/webserv.hpp"

/*
void testServer_findRoute(Server &server, const Str &route)
{
	Route *_route = server.findRoute(route);
	if (_route) std::cout << route + " is available" << std::endl;
	else        std::cout << route + " not available" << std::endl;
}

void testServer_myErrorPage(Server &server, int code)
{
	Str path = server.myErrorPg(code);
	if (path.empty()) std::cout << "No error page for " << code << std::endl;
	else        	  std::cout << code << " " << path << std::endl;
}

void testRoute_isMethodAllow(Route *route, const Str &method)
{
	if (route->isMethodAllow(method)) std::cout << method << " allowed" << std::endl;
	else							  std::cout << method << " not allowed" << std::endl;
}

void testRoute_runWithCGI(Route *route, const Str &file)
{
	if (route->runWithCGI(file)) std::cout << file + " run CGI" << std::endl;
	else							  std::cout << file + " not CGI" << std::endl;
}

void testClient_parseReq(Client &client, Str &req)
{
	if (client.appendReq(&req[0], req.size()))
	{
		std::cout << "OK. Detect complete request\n" << std::endl;
		client.showData();
	}
	else std::cout << "Request is incomplete" << std::endl;
}

int main()
{
	Server server = serverA();

	server.showData();

	std::cout << YELLOW "\nServer: findRoute()" RESET << std::endl;
	testServer_findRoute(server, "/archive/");
	testServer_findRoute(server, "/invalid/");
	testServer_findRoute(server, "/upload");

	std::cout << YELLOW "\nServer: myErrorPg()" RESET << std::endl;
	testServer_myErrorPage(server, 500);
	testServer_myErrorPage(server, 400);
	testServer_myErrorPage(server, 405);
	testServer_myErrorPage(server, 404);

	Route *route_1 = server.findRoute("/test/");
	Route *route_2 = server.findRoute("/delete/");

	std::cout << YELLOW "\nRoute: isMethodAllow()" RESET << std::endl;
	testRoute_isMethodAllow(route_1, "GET");
	testRoute_isMethodAllow(route_1, "POST");
	testRoute_isMethodAllow(route_1, "PATCH");
	testRoute_isMethodAllow(route_2, "GET");
	testRoute_isMethodAllow(route_2, "DELETE");

	std::cout << YELLOW "\nRoute: runWithCGI()" RESET << std::endl;
	testRoute_runWithCGI(route_1, "hello.py");
	testRoute_runWithCGI(route_2, "dummy.py");
	testRoute_runWithCGI(route_2, "morning.php");

	Client client(server);

	Str req1 = 
		"POST /upload/test.txt HTTP/1.1\r\n"
		"Host: 127.0.0.1:8080\r\n"
		"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
		"Content-Type: application/x-www-form-urlencoded\r\n"
		"Content-Length: 28\r\n";

	Str req2 =
		"Accept: gf\r\n"
		"Connection: keep-alive\r\n"
		"\r\n"
		"username=";
	
	Str req3 = "test&password=12345";

	std::cout << YELLOW "\nClient: Parse complete request()" RESET << std::endl;
	testClient_parseReq(client, req1);
	testClient_parseReq(client, req2);
	testClient_parseReq(client, req3);

	std::cout << YELLOW "\nClient: reuseFd()" RESET << std::endl;
	client.reuseFd();
	client.showData();
}
*/