#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <cerrno>
#include <filesystem>
#include <dirent.h> 
#include <sys/stat.h>
#include <cstring>
#include <unistd.h> 
#include "Server.hpp"

typedef std::string Str;
typedef std::map<Str, bool> DirItems;
typedef std::vector<char> Binary;
typedef std::map<Str, Str> Header;

class Client
{
    private:
        Server      server;
        
        Str         data;
        Header      header;
        Route       *route;
        
        Binary      reply;
        int         byteSend; 

    public:
        Client(const Server &_server_);

        size_t bodySize() const {return data.size();}
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

        Str         _filePath;
        Str         _uploadDir;
        const Str   _myErrorPg(const int &code) const;

        //////////////////////////////////////////////////////////
        ///                     MY USAGE                      ////
        //////////////////////////////////////////////////////////

        // ******** POLLING STAGE ***********

        bool    appendReq(Str request);
        bool    isHeadReady() const;
        
        // ****** PROCESS REQUEST *******
        
        bool    isBodyWithinLimit() const;
        bool    isBodyMatchLen()const;
        bool    haveRoute() const;
        bool    isAutoIndex() const;
        bool    isMethodAllow() const;
        bool    isCGI() const;

        // ******* POLLOUT STAGE ******
        
        Binary respond(){return this->reply;}
        //const char *respond() {return &reply[0];}
        bool        isKeepAlive() const; 
        void        reUseFd();

        // ****** ADDITIONAL  **********
        
        void        showData();
        const Str   getHost() const;
        const Str   getDefaultFile() const;

        ///////////////////////////////////////////////////////////
        ///               PUT YOUR RESPONSE HERE                /// 
        ///////////////////////////////////////////////////////////

        void resFectchFile();
        void resDirList();
        void resDefaultError(int code);        
        void resError(int code);
        void resRedirectAddSlash();
        void resRedirectTo();
        void resSaveFile();
        void resDeleteFile();
        void resDeleteDir();

        void handleCGI()
        {std::cout << "Run CGI" << std::endl;}
};

#endif