#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include "Server.hpp"

typedef std::string Str;
typedef std::map<Str, Str> Header;

class Client
{
    private:
        Server    server;
        
        Str        data;
        Header     header;
        Route      *route;
        
        Str        reply;
        int        byteSend; 

    public:
        Client(const Server &_server_);

        bool appendReq(char &request);
        bool isHeadReady() const;

        //////////////////////////////////////////////////////////
        ///            ALL INFO YOU NEED IS IN HERE           ////
        //////////////////////////////////////////////////////////
    
        Str     _method;
        Str     _path;
        Str     _version;
        Str     _contentType;
        int     _contentLen;
        Str     _file;
        Str     _redirect;

        // Absolute path root + subdir
        Str         _filePath;
        Str         _uploadDir;
        Str         _cgiPath;
        const Str   _myErrorPg(const int &code) const;

        ///////////////////////////////////////////////////////////
        ///     GETTER FUNCTION : USE AFTER PARSE REQUEST       /// 
        ///////////////////////////////////////////////////////////
        
        int   getMaxBodySize() const;

        bool  iskeepAlive() const;   
        bool  isAutoIndex() const;
        bool  isMethodAllow(const Str &method) const;

        const Str   getHost() const;
        const Str   getDefaultFile() const;
        const Str   runWithCGI(const Str &file) const;
        {
            std::cout << "\nTry route /archive/ " << std::endl; 
            this->route = this->server.findRoute("/archive/"); 
        }
};

#endif