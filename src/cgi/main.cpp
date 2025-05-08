#include <iostream>
#include <map>
#include <vector>
#include "CGIHandler.hpp"

int main() {
	std::map<std::string, std::string> env;
	env["REQUEST_METHOD"] = "POST";
	env["CONTENT_LENGTH"] = "13";
	env["CONTENT_TYPE"] = "application/x-www-form-urlencoded";
	env["PATH_INFO"] = "/cgi-bin/hello.py";  // path to script

	std::string requestBody = "\nname=Ryan\nage=26\nprofession=sarcasm"; // example request body
	std::vector<std::string> cgiPaths;
	cgiPaths.push_back("hello.py");

	try {
		CGIHandler handler(env, requestBody, cgiPaths);
		std::string response = handler.Execute();
		std::cout << "CGI response:\n" << response << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "CGI execution failed: " << e.what() << std::endl;
	}

	return 0;
}
