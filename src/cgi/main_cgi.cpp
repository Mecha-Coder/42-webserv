#include <iostream>
#include <map>
#include <vector>
#include <sys/time.h>
#include "CGIHandler.hpp"

int main() {
	struct timeval start, end;
	std::map<std::string, std::string> env;
	env["REQUEST_METHOD"] = "POST";
	env["CONTENT_LENGTH"] = "13";
	env["CONTENT_TYPE"] = "application/x-www-form-urlencoded";
	env["PATH_INFO"] = "/cgi-bin/hello.py"; // path to script
	// env["PATH_INFO"] = "/cgi/cgi-bin/hello.py"; // error test

	std::string requestBody = "\nname=Ryan\nage=26\nprofession=sarcasm"; // example request body
	std::vector<std::string> cgiPaths;
	cgiPaths.push_back("hello.py");

	CGIHandler handler(env, "", cgiPaths);
	gettimeofday(&start, NULL);
	std::string response = handler.Execute();
	std::cout << "CGI response:\n" << response << std::endl;
	gettimeofday(&end, NULL);
	long seconds = end.tv_sec - start.tv_sec;
	long microseconds = end.tv_usec - start.tv_usec;
	long elapsedMilli = (seconds * 1000) + (microseconds / 1000);

	std::cout << "CGI script took " << elapsedMilli << " ms to respond." << std::endl;

	return 0;
}

// TODO:
// status line handling for http line - refer miro
// throw error to resError(500)
// append response to reply (string) var
