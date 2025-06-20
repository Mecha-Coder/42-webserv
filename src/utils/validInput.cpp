#include "../include/webserv.hpp"

size_t strToSizeT(const Str s);

Str validMethod(Str s)
{
	if (s == "GET" || s == "DELETE" || s == "POST" || s == "PUT" 
		|| s == "PATCH" || s == "TRACE" || s == "OPTIONS" || s == "HEAD" )
	{;}
	else
		throw std::runtime_error("HTTP Method - invalid input");
	
	return s;
}

Str validCGI(Str s)
{
	if (s.empty() || s[0] != '.')
		throw std::runtime_error("CGI extension - must start with '.'");

	for (size_t i = 1; i < s.length(); ++i)
	{
		if (!std::islower(s[i]) && !std::isdigit(s[i]))
			throw std::runtime_error("CGI extension - invalid character");
	}

	return s;
}


int validErrCode(Str key)
{
	int code = static_cast<int>(strToSizeT(key));
	if (code < 400 || code > 599)
		throw std::runtime_error("Error page - invalid error code");
	return code;
}