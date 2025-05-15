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

        ///////////////////////////////////////////////////////////
        ///               PUT YOUR RESPONSE HERE                /// 
        ///////////////////////////////////////////////////////////

        void resError(int code)
        {
            if (this->_myErrorPg(code).empty())
                resDefaultError();
            else
                std::cout << "Respond custom error page" << std::endl;   
        }
        
        void resDefaultError()
        {std::cout << "No custom page. Use default error page" << std::endl;}

        void resDirList()
        {std::cout << "List of files / folder in dir" << std::endl; }

        void resFectchFile()
        {std::cout << "Fetch file. filePath + file" << std::endl; }

        void resSaveFile()
        {std::cout << "Post request, save file to specified dir" << std::endl;}

        void resRedirect_1()
        {std::cout << "308 preserve method and the request body during redirection" << std::endl;}

        void resRedirect_2()
        {std::cout << "301 permanent redirect" << std::endl;}
        
        void resDeleteFile()
        {std::cout << "Delete file" << std::endl;}

        void resDeleteDir()
        {std::cout << "Delete Directory" << std::endl;}

        void handleCGI()
        {std::cout << "Run CGI" << std::endl;}
};

#endif