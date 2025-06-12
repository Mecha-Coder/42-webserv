#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <iostream>
#include <string>
#include <vector>
#include "constant.hpp"

typedef std::string Str;
typedef std::vector<Str> List;

class Route
{
    private:
        List    _methods;
        List    _cgi; 
        
    public:
        Str   _path;
        Str   _redirect;
        Str   _defaultFile;
        bool  _uploadDir;
        bool  _autoIndex;

        Route (
            const List &methods,
            const List &cgi,
            const Str &path,
            const Str &redirect,
            const Str &defaultFile,
            const bool &uploadDir,
            const bool &autoIndex
        );
        
        bool    methodAllow(const Str &method) const;
        bool    runCGI(const Str &uri) const;
};

#endif