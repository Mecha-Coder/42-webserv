#include "../../include/Route.hpp"

Route::Route(
    const Str &uri,
    const Str &root,
    const Str &redirect,
    const Str &defaultFile,
    const Str &uploadDir,
    const bool &autoIndex,
    const Method &methods,
    const CGI &cgi
)
: _methods(methods), _cgi(cgi), _uri(uri), _root(root),
_redirect(redirect),  _uploadDir(uploadDir), _autoIndex(autoIndex),
_defaultFile(defaultFile) {}

/////////////////////////////////////////////////////////////////////////

bool Route::isMethodAllow(const Str &method) const
{
    Method::const_iterator i = this->_methods.begin();
    for (; i != this->_methods.end(); i++)
    {
        if (*i == method)
            return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////

bool Route::runWithCGI(const Str &file) const
{
    Str extension = file.substr(file.rfind("."));
    CGI::const_iterator i = _cgi.begin();
    for (; i != this->_cgi.end(); i++)
    {
        if (*i == extension)
            return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////

void Route::showData() const
{
    std::cout << "\tRoute data: " << _uri << "\n"
              << "\t************************\n"
              << "\tRoot      : [" << _root << "]\n"
              << "\tRedirect  : [" << _redirect << "]\n"
              << "\tUpload Dir: [" << _uploadDir << "]\n"
              << "\tAuto Index: " << (_autoIndex? "NO" : "OFF") << "\n"
              << "\tDefau File: [" << _defaultFile << "]\n"
              << "\tMethod    : ";
    

    for (size_t i = 0; i < _methods.size(); i++)
        std::cout << _methods[i] << ", ";
    
    std::cout << "\n\tCGI       : ";
    
    for (size_t j = 0; j < _cgi.size(); j++)
        std::cout << _cgi[j] << ", ";
    
    std::cout << "\n" << std::endl; 
}