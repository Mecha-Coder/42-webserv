#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>

typedef std::string Str;
typedef std::vector<Str> Method;
typedef std::vector<Str> CGI;

class Route
{
    private:
        Method  _methods;
        CGI     _cgi; 
        
    public:
        const Str     _uri;
        const Str     _root;
        const Str     _redirect;
        const Str     _uploadDir;
        const bool    _autoIndex;
        Str           _defaultFile;

        Route (
            const Str &uri,
            const Str &root,
            const Str &redirect,
            const Str &defaultFile,
            const Str &uploadDir,
            const bool &autoIndex,
            const Method &methods,
            const CGI &cgi
        );
        
        bool        isMethodAllow(const Str &method) const;
        const Str   runWithCGI(const Str &file) const;
        void        showData() const;
};

#endif