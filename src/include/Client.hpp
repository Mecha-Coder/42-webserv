#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "constant.hpp"
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

#include "ServerManager.hpp"
#include "CGIHandler.hpp"

template <typename T> Str toStr(T value);

typedef std::string Str;
typedef std::vector<Str> List;
typedef std::map<Str, bool> DirItems;
typedef std::map<Str, Str> Header;

class Client
{
	private:
		// Server data ---------------------------------------
		Server			*_server;
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
		Str				_host;
		Str				_fullPath;
		bool			_keepAlive;

		Client();

		/*************************************************/
		// Response: Chunk response & check FD reuse
		/*************************************************/
		
		void		reuseFd();
		bool		trackReply(size_t count);
		const char *getReply(size_t &remainSize);
		

		/*************************************************/
		// Request: Append and parse
		/*************************************************/

		bool	appendReq(const char *request, size_t byteRead, ServerManager &sManager);
		void	parseHead(Str info);
		void	parseLine(Str line);
		void 	getServerData(ServerManager &sManager);


		/*************************************************/
		// Checker
		/*************************************************/

		bool isRequestLine_Malform() const;
		bool isContentHeader_Invalid() const;
		bool isBody_ExceedLimit() const;

		bool isURI_noRoute() const;
		bool isRedirect_True() const;
		bool isMethod_Illegal() const;

		bool isAutoIndex_On() const;
		bool isReq_Upload() const;

		int	 getResourceType() const;
		bool noDefaultFile();
		bool isURI_noSlash() const;
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