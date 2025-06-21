#include "../../include/Client.hpp"

bool isDirExist(const Str dir);
bool isFileExist(const Str path);

/*******************************************************************************/
// Validate Request
/*******************************************************************************/

bool Client::isRequestLine_Malform() const
{ return (_method.empty() || _uri.empty() ||  _host.empty() || _version != "HTTP/1.1"); }

bool Client::isContentHeader_Invalid() const
{ return (_contentType.empty() || _contentLen == 0 || _data.size() != _contentLen); }

bool Client::isBody_ExceedLimit() const
{ return (_data.size() > _server->_clientBody); }


/*******************************************************************************/
// Route Requirement
/*******************************************************************************/

bool Client::isURI_noRoute() const
{return _route == NULL;}

bool Client::isRedirect_True() const
{ return _route->_redirect.empty() == false; }

bool  Client::isMethod_Illegal() const
{ return !_route->methodAllow(_method); }

/*******************************************************************************/
// Do GET 
/*******************************************************************************/

bool Client::isAutoIndex_On() const
{ return _route->_autoIndex; }

/*******************************************************************************/
// Do POST
/*******************************************************************************/

bool Client::isReq_Upload() const
{
    return (_route->_uploadDir &&
        (_contentType.find("multipart/form-data; boundary=") != _contentType.npos));
}

/*******************************************************************************/
// Others
/*******************************************************************************/

bool Client::noDefaultFile()
{
    Str path;
    List &index = _route->_index; 

    for (size_t i=0; i < index.size(); i++)
    {
        path = _fullPath + index[i];

        if (!index[i].empty() && isFileExist(path))
        {
            _fullPath = path;
            return false;
        }
    }
    return true;
}

int Client::getResourceType() const
{
    if (isDirExist(_fullPath)) return TYPE_FOLDER;
    if (isFileExist(_fullPath)) return TYPE_FILE;
    return -1;
}

bool   Client::isCGI() const
{ return _route->runCGI(_fullPath); }

bool Client::isURI_noSlash() const
{ return *_uri.rbegin() != '/'; }
