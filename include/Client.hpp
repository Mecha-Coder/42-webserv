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
#include "constant.hpp"

template <typename T> Str toStr(T value);

typedef std::string Str;
typedef std::map<Str, bool> DirItems;
typedef std::vector<Str> List;
typedef std::map<Str, Str> Header;

class Client
{
	private:
		// Server data ---------------------------------------
		const Server	_server;
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
		Str				_method;
		Str				_uri;
		Str				_path;
		Str				_file;
		Str				_host;
		bool			_keepAlive;

		Client(const Server &_server_);

		bool    appendReq(Str request);
		void	reUseFd();
		void	showData();
		
		
		/*************************************************/
		// Checker
		/*************************************************/

		// Validate request

		bool isRequestLine_Malform() const;
		bool isContentHeader_Invalid() const;
		bool isBody_ExceedLimit() const;

		// Route Requirement

		bool isRedirect_True() const;
		bool isPath_noSlash() const;
		bool isPath_noRoute() const;
		bool isPath_noExist() const;
		bool isMethod_Illegal() const;

		// GET request
		
	

		// POST request
		bool isReq_Upload() const;

		// DELETE request

		// Others
		bool isFile_Empty() const;
		bool isAutoIndex_On() const;
		bool isFile_noExist() const;
		bool noDefaultFile();
		bool isHeadReady() const;
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
		// Response Template
		/*************************************************/

		Str tmplErrDefault(Code code);
		Str tmplErrCustom(Code code, const Str &body);
		Str tmplDirList(const Str &path, const DirItems &items);
		Str tmplFetch(const Str &filename, const Str &body);
		Str tmplSave(List saveFile);
		Str tmplDelete(const Str &item);
		Str tmplRedirect(Code code, const Str &redirectTo);
};

#endif