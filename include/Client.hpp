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
#include "CGIHandler.hpp"

void logMsg(const Str &where, Str action, int state);

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

        void resFetchFile();
        void resDirList();
        void resDefaultError(int code);        
        void resError(int code);
        void resRedirectAddSlash();
        void resRedirectTo();
        void resSaveFile();
        void resDeleteFile();
        void resDeleteDir();

        void handleCGI()
        {
            logMsg(getHost() +  " | handleCGI", "Run CGI", 1);

            std::vector<Str> path; path.push_back(this->_filePath + this->_file);

            Header env;
            env["REQUEST_METHOD"] = this->_method;
            env["CONTENT_LENGTH"] = this->_contentLen;
            env["CONTENT_TYPE"] = this->_contentType;
            env["PATH_INFO"] = this->_filePath + this->_file;

            CGIHandler obj(env, this->data, path);

            try 
            {
                Str result = obj.Execute();

                std::cout << "CGI Result: \n=======\n" << result << std::endl;
                this->reply.insert(this->reply.end(), result.begin(), result.end());
            }
            catch(std::exception &e)
            {
                std::cerr << "CGIHandler error: " << e.what() << std::endl;
                this->resError(500);
            }
            
        }
};

#endif