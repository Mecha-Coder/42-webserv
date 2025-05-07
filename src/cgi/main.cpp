#include <iostream>
#include <map>
#include <vector>
#include "CGIHandler.hpp"  // assuming this is in a header

int main() {
	std::map<std::string, std::string> env;
	env["REQUEST_METHOD"] = "POST";
	env["CONTENT_LENGTH"] = "13";
	env["CONTENT_TYPE"] = "application/x-www-form-urlencoded";
	env["PATH_INFO"] = "/www/cgi-bin/hello.py";  // matched by your logic

	std::string requestBody = "\nname=Ryan\nage=26\nprofession=sarcasm"; // example request body
	std::vector<std::string> cgiPaths;
	cgiPaths.push_back("hello.py"); // should match filename

	try {
		CGIHandler handler(env, requestBody, cgiPaths);
		std::string response = handler.Execute();
		std::cout << "CGI response:\n" << response << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "CGI execution failed: " << e.what() << std::endl;
	}

	return 0;
}
