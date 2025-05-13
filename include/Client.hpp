#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <map>
#include "Server.hpp"

typedef std::string Str;
typedef std::map<Str, Str> Header;

class Client
{
    private:
        const Server    server;
        
        Str             data;
        Header          header;
        Route           *route;
        
        Str             reply;
        int             byteSend; 

    public:
        Client(const Server &_server_);

        //////////////////////////////////////////////////////////
        ///            ALL INFO YOU NEED IS IN HERE           ////
        //////////////////////////////////////////////////////////
    
        Str     _method;
        Str     _path;
        Str     _version;
        Str     _contentType;
        int     _contentLen;
        Str     _redirect;
        Str     _file;

        // Absolute path root + subdir
        Str         _filePath;
        Str         _uploadDir;
        Str         _cgiPath;
        const Str   _myErrorPg(const int &code) const;

        ///////////////////////////////////////////////////////////
        ///                GETTER FUNCTION                      ///
        ///////////////////////////////////////////////////////////
        
        const int   getMaxBodySize() const;

        const bool  iskeepAlive() const;   
        const bool  isAutoIndex() const;
        const bool  isMethodAllow(const Str &method) const;

        const Str   getHost() const;
        const Str   getURI() const;
        const Str   getDefaultFile() const;
        const Str   runWithCGI(const Str &file) const;
};

#endif