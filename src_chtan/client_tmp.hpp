#include <iostream>
#include <string>
#include <map>
#include <dirent.h>  // For directory operations
#include <sys/stat.h> // For stat()
#include <cerrno>    // For errno
#include <cstring>   // For strerror()

typedef std::string Str;
typedef std::map<Str, Str> Header;

class Client
{
    private:
        Str     data;
        Header  header;
        std::string _default_path;  //1
        std::string _file_path; //2
        std::string _upload_path; //3

    public:
        Str     method;
        Str     path;
        Str     version;
        Str     host;
        Str     file;
        Str     contentType;
        int     contentLen;
        bool    keepAlive;
        int     error_code;


    Client(const Client &tocopy);
    Client &operator=(const Client &tocopy);
    ~Client(void);
    Client();
    std::string get_path(int which) const;


    // bool    headderReady();
    // bool    appendReq(Str request);

    // Str     *body();
    // int     bodySize();

    // void    showDetails();
};
