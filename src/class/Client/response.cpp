#include "../../include/Client.hpp"

bool readDir(const Str &path, DirItems &items);
bool readFile(const Str& filename, Str &content);

bool saveBodyPart(Str &bodyPart, const Str &saveHere, Str &filename);

bool deleteFile(const Str& filePath);
bool deleteDir(const Str& path);

void logError(const Str &where, Str action);
void logAction(const Str &where, Str action);

/////////////////////////////////////////////////////////////////////////////////////

bool Client::resDefaultError(Code code, const Str &msg)
{
    Str where = _host + " | resDefaultError";

    _reply = tmplErrDefault(code);
    return (logError(where, "Respond: " + toStr(code) + ": " + msg), false);
}

//////////////////////////////////////////////////////////////////////////////////////

bool Client::resError(Code code, const Str &msg)
{
    Str where = _host + " | resError";

    Str path, body;
    
    if (_server)
        path = _server->myErrorPg(code);
    if (!path.empty() && readFile(path, body))
    {
        _reply = tmplErrCustom(code, body);
        return (logError(where, "Respond: " + toStr(code) + ": " + msg), false);
    }
    return resDefaultError(code, msg);
}

//////////////////////////////////////////////////////////////////////////////////////

bool Client::resDirList()
{
    Str where = _host + " | resDirList";
    
    DirItems items;

    if (readDir(_fullPath, items))
    {
        _reply = tmplDirList(_uri, items);
        return (logAction(where, "Respond: 200: Return directory list for " + _uri), true);
    }
    return resError(_500, "resDirList: Failed directory list for " + _uri);
}

/////////////////////////////////////////////////////////////////////////////////////

bool Client::resFetchFile()
{   
    Str where = _host + " | resFetchFile";

    Str body;
    Str filename = _fullPath.substr(_fullPath.rfind("/") + 1);

    if (readFile(_fullPath, body))
    {
        _reply = tmplFetch(filename, body);
        return (logAction(where, "Respond: 200: Fetch " + _uri), true);
    }
    return resError(_500, "resFetchFile: Fail to read " + _uri);
}

/////////////////////////////////////////////////////////////////////////////////////

bool Client::resAddSlash()
{
    Str where = _host + " | resAddSlash";

    _reply = tmplRedirect(_308, _uri + "/");
    return (logAction(where, "Respond: 308: Redirect add slash \"/\" -> " + _uri), false);
}

/////////////////////////////////////////////////////////////////////////////////////

bool Client::resRedirect()
{
    Str where = _host + " | resRedirect";

    _reply = tmplRedirect(_301, _route->_redirect);
    return (logAction(where, "Respond: 301: Redirect to " +_route->_redirect), false);
}

/////////////////////////////////////////////////////////////////////////////////////

bool Client::resSaveFile()
{
    Str where = _host + " | resSaveFile";
    
    size_t pos;
    List fileSaved;
    Str bodyPart, boundary, filename;
    
    pos      = _contentType.find("boundary=") + 9;
    boundary = "--" + _contentType.substr(pos);

    pos = 0;
    while (
        _data.erase(0, pos + boundary.size() + 2), 
        pos = _data.find(boundary), 
        pos != _data.npos
    ) {
        bodyPart = _data.substr(0, pos - 2);
        
        if (saveBodyPart(bodyPart, _fullPath, filename))
        {
            logAction(where, filename + " saved");
            fileSaved.push_back(filename);
        }
        else logError(where, filename + " failed to save");
    }

    if (fileSaved.empty())
        return resError(_500, "resSaveFile: No files were saved");

    _reply = tmplSave(fileSaved);
    return (logAction(where, "Respond: 201"), true);
}

/////////////////////////////////////////////////////////////////////////////////////

bool Client::resDeleteFile()
{
    Str where = _host + " | resDeleteFile";

    if (deleteFile(_fullPath))
    {
        _reply = tmplDelete(_uri);
        return (logAction(where, "Respond: 204: Deleted " + _uri), true);
    }
    return resError(_500, "resDeleteFile: Failed to delete " + _uri);
}

/////////////////////////////////////////////////////////////////////////////////////

bool Client::resDeleteDir()
{
    Str where = _host + " | resDeleteDir";

    if (deleteDir(_fullPath))
    {
        _reply = tmplDelete(_uri);
        return (logAction(where, "Respond: 204: Deleted folder " + _uri), true);
    }
    return resError(_500, "resDeleteDir: Failed to delete folder " + _uri);
}

/////////////////////////////////////////////////////////////////////////////////////

bool validCGIRespond(const Str &s)
{
    return (
        s.find("HTTP/1.1") != s.npos 
        && s.find("Content-Type") != s.npos 
        && s.find("Content-Length") != s.npos 
        && s.find("\r\n\r\n") != s.npos
    );
}

/////////////////////////////////////////////////////////////////////////////////////

bool Client::resCGI(const Str &msg)
{
    Str where = _host + " | resCGI";

    List path; 
    path.push_back(_fullPath);

    Header env;
    env["REQUEST_METHOD"] = _method;
    env["PATH_INFO"]      = _fullPath;
    env.insert(_header.begin(), _header.end());

    CGIHandler handler(env, &_data, path);

    try
    { 
        Str result = handler.Execute();
        
        if (validCGIRespond(result))
        {
            _reply = result;
            return (logAction(where, msg + ": Executed CGI for " + _uri), true); 
        }
        throw std::runtime_error("Invalid CGI respond");
    }
    catch(std::exception &e) 
    { 
        resError(_500, msg + ": resCGI: " + Str(e.what() + Str(" >> ") + _uri)); 
    }
    return false;
}
