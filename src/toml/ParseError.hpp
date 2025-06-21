#pragma once

#include <string>

class ParseError {
public:
	ParseError();
	ParseError(std::string, int);

	std::string asStr() {
		return "ParseError: [" + message + "] at line " + std::to_string(line);
	}

private:
	std::string message;
	int line;
};
