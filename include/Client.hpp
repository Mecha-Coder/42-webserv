#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <map>

typedef std::string Str;
typedef std::map<Str, Str> Header;

class Client
{
    private:
        Str     data;
        Header  header; 
    
    public:
        Str     method;
        Str     path;
        Str     version;
        Str     host;
        Str     file;
        Str     contentType;
        int     contentLen;
        bool    keepAlive;

    Client();
    
    void resError();

    bool    headderReady();
    bool    appendReq(Str request);

    Str     *body();
    int     bodySize();

    void    showDetails();
};

#endif