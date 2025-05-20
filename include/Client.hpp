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
        const Str   _myErrorPg(const int &code) const;

        ///////////////////////////////////////////////////////////

        // ******** POLLING STAGE ***********

        bool    appendReq(Str request);
        bool    isHeadReady() const;
        
        // ****** PROCESS REQUEST *******
        
        bool    bodyWithinLimit() const;
        bool    bodySizeMatch()const;
        bool    haveRoute() const;

        // *****  

        const char *respond() {return reply.c_str();}

        // UTILS
        void reUseFd();
        void showData();



        bool  isKeepAlive() const;   
        bool  isAutoIndex() const;
        bool  isMethodAllow() const;
        bool   runWithCGI() const;

        const Str   getHost() const;
        const Str   getDefaultFile() const;

        ///////////////////////////////////////////////////////////
        ///               PUT YOUR RESPONSE HERE                /// 
        ///////////////////////////////////////////////////////////

        void resError(int code)
        {
            if (this->_myErrorPg(code).empty())
                resDefaultError(code);
            else
                std::cout << code <<": Respond custom error page" << std::endl;   
        }
        
        void resDefaultError(int code)
        {std::cout << code << ": No custom page. Use default error page" << std::endl;}

        void resDirList()
        {std::cout << "List of files / folder in dir" << std::endl; }

        void resFectchFile()
        {std::cout << "Fetch file. filePath + file" << std::endl; }

        void resSaveFile()
        {std::cout << "Post request, save file to specified dir" << std::endl;}

        void resRedirectAddSlash()
        {std::cout << "308 preserve method and the request body during redirection" << std::endl;}

        void resRedirectTo()
        {std::cout << "301 permanent redirect" << std::endl;}
        
        void resDeleteFile()
        {std::cout << "Delete file" << std::endl;}

        void resDeleteDir()
        {std::cout << "Delete Directory" << std::endl;}

        void handleCGI()
        {std::cout << "Run CGI" << std::endl;}
};

#endif