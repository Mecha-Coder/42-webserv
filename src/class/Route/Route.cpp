#include "../../include/Route.hpp"

/////////////////////////////////////////////////////////////////////////

Route::Route(
    const List &methods,
    const List &cgi,
    const Str &path,
    const Str &redirect,
    const Str &defaultFile,
    const bool &uploadDir,
    const bool &autoIndex
) : 
    _methods(methods), 
    _cgi(cgi), 
    _path(path),
    _redirect(redirect),  
    _defaultFile(defaultFile),
    _uploadDir(uploadDir), 
    _autoIndex(autoIndex)
{}

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