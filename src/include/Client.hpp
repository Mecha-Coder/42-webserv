#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <filesystem>
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>
#include <cerrno>
#include <map>

#include <sys/stat.h>
#include <dirent.h> 
#include <unistd.h>

#include "Server.hpp"
#include "CGIHandler.hpp"

template <typename T> Str toStr(T value);

typedef enum t_code
{
    _200 = 200, 
    _201 = 201, 
    _204 = 204, 
    _301 = 301, 
    _308 = 308,
    
    _400 = 400,
    _403 = 403,
    _404 = 404, 
    _405 = 405, 
    _410 = 410, 
    _413 = 413,
    _500 = 500 
}   Code;

typedef std::string Str;
typedef std::map<Str, bool> DirItems;
typedef std::vector<Str> List;
typedef std::map<Str, Str> Header;

class Client
{
	private:
		// Server data ---------------------------------------
		Server			&_server;
		Route			*_route;

		// Request data -------------------------------------
		Str				_data;
		Header			_header;

		Str				_version;
		Str				_contentType;
		size_t			_contentLen;

		// Response data ------------------------------------
		Str				_reply;
		size_t			_byteSent; 

	public:
		// Request data - quick access to data --------------
		Str				_method;
		Str				_uri;
		Str				_path;
		Str				_file;
		Str				_host;
		bool			_keepAlive;

		Client(Server &_server_);
		void	showData();

		/*************************************************/
		// Response: Chunk response & check FD reuse
		/*************************************************/
		
		void		reuseFd();
		bool		trackReply(size_t count);
		const char *getReply(size_t &remainSize);
		

		/*************************************************/
		// Request: Append and parse
		/*************************************************/

		bool	appendReq(const char *request, size_t byteRead);
		void	parseHead(Str info);
		void	parseLine(Str line);

		/*************************************************/
		// Checker
		/*************************************************/

		bool isRequestLine_Malform() const;
		bool isContentHeader_Invalid() const;
		bool isBody_ExceedLimit() const;

		bool isRedirect_True() const;
		bool isPath_noSlash() const;
		bool isPath_noRoute() const;
		bool isPath_noExist() const;
		bool isMethod_Illegal() const;
	
		bool isAutoIndex_On() const;
		bool isReq_Upload() const;
		bool isFile_Empty() const;
		bool isFile_noExist() const;
		bool noDefaultFile();
		bool isCGI() const;

		/*************************************************/
		// Response
		/*************************************************/

		bool resDefaultError(Code code, const Str &msg);        
		bool resError(Code code, const Str &msg);
		bool resCGI(const Str &msg);

		bool resSaveFile();
		bool resDeleteFile();
		bool resDeleteDir();
		bool resAddSlash();
		bool resRedirect();
		bool resFetchFile();
		bool resDirList();


		/*************************************************/
		// Template for response 
		/*************************************************/

		Str tmplErrDefault(Code code);
		Str tmplErrCustom(Code code, const Str &body);
		Str tmplDirList(const Str &path, const DirItems &items);
		Str tmplFetch(const Str &filename, const Str &body);
		Str tmplSave(List saveFile);
		Str tmplDelete(const Str &item);
		Str tmplRedirect(Code code, const Str &redirectTo);
};

#include "../utils/toStr.tpp"

#endif