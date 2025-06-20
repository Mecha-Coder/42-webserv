#include "../../include/Route.hpp"

Str trim(const Str s);
bool strBool(Str s);
Str validMethod(Str s);
Str validCGI(Str s);

/////////////////////////////////////////////////////////////////////////

Route::Route(LocationConfig &data) 
{
    size_t i;

    if (data.prefix.empty())
        throw std::runtime_error("Mandatory data - prefix missing");
    
    _path      = trim(data.prefix);
    _uploadDir = strBool(trim(data.upload));
    _autoIndex = strBool(trim(data.autoindex));

    if (!data.redirect.empty())
        _redirect  = trim(data.redirect[0]);

    if (data.allowed_methods.empty())
        _methods.push_back("GET");
    else
    {
        for (i = 0; i < data.allowed_methods.size(); i++)
            _methods.push_back(validMethod(trim(data.allowed_methods[i])));
    }
    
    for (i = 0; i < data.cgi.size(); i++)
        _cgi.push_back(validCGI(trim(data.cgi[i])));

    for (i = 0; i < data.index.size(); i++)
        _index.push_back(trim(data.index[i]));

}

/////////////////////////////////////////////////////////////////////////

bool Route::methodAllow(const Str &method) const
{
    List::const_iterator i;

    for (i = _methods.begin(); i != _methods.end(); i++)
    {
        if (*i == method)
            return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////

bool Route::runCGI(const Str &uri) const
{
    size_t pos;
    Str extension;
    List::const_iterator i;
    
    if ((pos = uri.rfind(".")) == uri.npos)
        return false;

    extension = uri.substr(pos);

    for (i = _cgi.begin(); i != _cgi.end(); i++)
    {
        if (*i == extension)
            return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////