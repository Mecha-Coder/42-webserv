#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include "constant.hpp"
#include "Config.hpp"

typedef std::string Str;
typedef std::vector<Str> List;

class Route
{
    private:
        List    _methods;
        List    _cgi;
        
    public:
        Str     _path;
        Str     _redirect;
        bool    _uploadDir;
        bool    _autoIndex;
        List    _index;

        Route (LocationConfig &data);
        
        bool    methodAllow(const Str &method) const;
        bool    runCGI(const Str &uri) const;
        void    showData();
};

#endif