#include "../../include/Client.hpp"
bool isFolderExist(const Str dir);
bool isFileExist(const Str path);

/////////////////////////////////////////////////////////////////////
// Validate Request
/////////////////////////////////////////////////////////////////////

bool Client::isRequestLine_Malform() const
{ return (_method.empty() || _uri.empty() || _version != "HTTP/1.1"); }

bool Client::isContentHeader_Invalid() const
{ return (_contentType.empty() || _contentLen == 0 || _data.size() == _contentLen); }

bool Client::isBody_ExceedLimit() const
{ return (_data.size() > _server._clientBodySize); }

/////////////////////////////////////////////////////////////////////
// Route Requirement
/////////////////////////////////////////////////////////////////////

bool Client::isPath_noSlash() const
{ return *_path.rbegin() != '/'; }

bool Client::isPath_noRoute() const
{return _route != NULL;}

bool Client::isRedirect_True() const
{ return _route->_redirect.empty() == false; }

bool Client::isPath_noExist() const
{ return !isFolderExist(_route->_root + _path); }

bool  Client::isMethod_Illegal() const
{ return !_route->isMethodAllow(_method); }

/////////////////////////////////////////////////////////////////////
// Do GET 
/////////////////////////////////////////////////////////////////////

bool Client::isFile_Empty() const
{ return _file.empty(); }

bool Client::noDefaultFile()
{
    if (_route->_defaultFile.empty())
        return true;
    _file = _route->_defaultFile;
    return false;
}

bool Client::isAutoIndex_On() const
{ return _route->_autoIndex; }

bool Client::isFile_noExist() const
{ return !isFileExist(_route->_root + _uri); }

///////////////////////////////////////////////////////////////////// 
// Others
/////////////////////////////////////////////////////////////////////

bool Client::isHeadReady() const
{ return (header.size() && !_method.empty()); }


bool   Client::isCGI() const
{ return _route->runWithCGI(_file); }


bool  Client::isKeepAlive() const
{   
    Header::const_iterator i = this->header.find("Connection");
    
    if (i != header.end())
    {
        if (i->second == "keep-alive") return true; 
    }
    return false;
}