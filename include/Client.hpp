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

        bool appendReq(Str request);
        bool isHeadReady() const;
        
        const char *respond() {return reply.c_str();}

        void reuseFd();
        void showData();

        //////////////////////////////////////////////////////////
        ///            ALL INFO YOU NEED IS IN HERE           ////
        //////////////////////////////////////////////////////////
    
        Str     _method;
        Str     _path;
        Str     _version;
        Str     _contentType;
        size_t  _contentLen;
        Str     _file;
        Str     _redirect;

        // Path root + subdir
        Str         _filePath;
        Str         _uploadDir;
        Str         _cgiPath;
        const Str   _myErrorPg(const int &code) const;

        ///////////////////////////////////////////////////////////
        ///     GETTER FUNCTION : USE AFTER PARSE REQUEST       /// 
        ///////////////////////////////////////////////////////////
        
        int   getMaxBodySize() const;

        bool  isKeepAlive() const;   
        bool  isAutoIndex() const;
        bool  isMethodAllow(const Str &method) const;

        const Str   getHost() const;
        const Str   getDefaultFile() const;
        const Str   runWithCGI(const Str &file) const;
};

#endif