#include "../include/webserv.hpp"

bool strBool(Str s)
{
	for (size_t i = 0; i < s.length(); ++i)
        s[i] = std::tolower(static_cast<unsigned char>(s[i]));
	
	if (s == "true" || s == "on")
		return true;

	if (s == "false" || s == "off" || s.empty())
		return false;

	throw std::runtime_error("Boolean data - invalid input");	
	return false;
}

size_t strToSizeT(const Str s)
{
    std::stringstream ss(s);
    size_t result = 0;
    ss >> result;

    if (ss.fail() || !ss.eof())
        throw std::runtime_error("Integer data - invalid input");
    return result;
}