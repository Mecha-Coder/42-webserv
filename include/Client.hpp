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

void logError(const Str &where, Str action);
void logAction(const Str &where, Str action);
Str toStr(T value);

typedef std::string Str;
typedef std::map<Str, bool> DirItems;
typedef std::vector<Str> List;
typedef std::map<Str, Str> Header;

/*
Attribute:
	Server data
	===========
		_server
			# Assigned at instantiation (fixed)
		*_route
			# NULL if no matching route for _path, 
			# This will be determine after parsing request header
	
	Request data
	============
		_data
			# Incoming request data is appended here
			# Once a complete HTTP header is received > it is 
			# parsed > storead in _header > and removed from _data
			# The remaining string left is the request body
		_header
			# Request header data (stored as key-value pair)

2)

*/
class Client
{
	private:
		// Server data -----------------------------
		const Server	_server;
		Route			*_route;

		// Request data ----------------------------
		Str				_data;
		Header			_header;

		Str				_version;
		Str				_contentType;
		size_t			_contentLen;

		// Response data --------------------------
		Str				_reply;
		size_t			_byteSent; 

	public:
		// Easy access to data ---------------------
		Str				_method;
		Str				_uri;
		Str				_path;
		Str				_file;
		Str				_host;
		bool			_keepAlive;

		// Checker - validate request -------------
		bool isRequestLine_Malform() const;
		bool isContentHeader_Invalid() const;
		bool isBody_ExceedLimit() const;

		// Checker - route requirement ------------
		bool isRedirect_True() const;
		bool isPath_noSlash() const;
		bool isPath_noRoute() const;
		bool isPath_noExist() const;
		bool isMethod_Illegal() const;

		// Checker - do GET request
		bool isFile_Empty() const;
		bool isAutoIndex_On() const;
		bool isFile_noExist() const;
		bool noDefaultFile();

		// Checker - do Post request --------------

		Str tmplErrDefault(Code code);
		Str tmplErrCustom(Code code, const Str &body);
		Str tmplDirList(const Str &path, const DirItems &items);
		Str tmplFetch(const Str &filename, const Str &body);
		Str tmplSave(List saveFile);
		Str tmplDelete(const Str &item);
		Str tmplRedirect(Code code, const Str &redirectTo);

		// Getter
		const Str &getRedirect() const;
	

		Client(const Server &_server_);
		size_t bodySize() const {return data.size();}

		// ******** POLLING STAGE ***********

		bool    appendReq(Str request);
		bool    isHeadReady() const;
		
		// ****** PROCESS REQUEST *******
		
		bool    isBodyWithinLimit() const;
		bool    isBodyMatchLen()const;
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
		
		

		///////////////////////////////////////////////////////////
		///               PUT YOUR RESPONSE HERE                /// 
		///////////////////////////////////////////////////////////

		bool resFetchFile();
		bool resDirList();
		bool resDefaultError(Code code);        
		
		bool resError(Code code);
		bool resAddSlash();
		bool resRedirect();

		bool resSaveFile();
		bool resDeleteFile();
		bool resDeleteDir();

		bool handleCGI()
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