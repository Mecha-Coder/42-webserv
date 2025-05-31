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
    
    path = _server.myErrorPg(code);
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

    if (readDir(_route->_root + _path, items))
    {
        _reply = tmplDirList(_path, items);
        return (logAction(where, "Respond: 200: Return directory list for " + _path), true);
    }
    return resError(_500, "resDirList: Failed directory list for " + _path);
}

/////////////////////////////////////////////////////////////////////////////////////

bool Client::resFetchFile()
{   
    Str where = _host + " | resFetchFile";

    Str body;

    if (readFile(_route->_root + _path + _file, body))
    {
        _reply = tmplFetch(_file, body);
        return (logAction(where, "Respond: 200: Fetch " + _path + _file), true);
    }
    return resError(_500, "resFetchFile: Fail to read " + _path + _file);
}

/////////////////////////////////////////////////////////////////////////////////////

bool Client::resAddSlash()
{
    Str where = _host + " | resAddSlash";

    _reply = tmplRedirect(_308, _path + "/");
    return (logAction(where, "Respond: 308: Redirect add slash \"/\" -> " + _path), false);
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
        
        if (saveBodyPart(bodyPart, _route->_root + _route->_uploadDir, filename))
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

    if (deleteFile(_route->_root + _path + _file))
    {
        _reply = tmplDelete(_path);
        return (logAction(where, "Respond: 204: Deleted " + _path + _file), true);
    }
    return resError(_500, "resDeleteFile: Failed to delete " + _path + _file);
}

/////////////////////////////////////////////////////////////////////////////////////

bool Client::resDeleteDir()
{
    Str where = _host + " | resDeleteDir";

    if (deleteDir(_route->_root + _path))
    {
        _reply = tmplDelete(_path);
        return (logAction(where, "Respond: 204: Deleted folder " + _path), true);
    }
    return resError(_500, "resDeleteDir: Failed to delete folder " + _path);
}

/////////////////////////////////////////////////////////////////////////////////////

bool Client::resCGI(const Str &msg)
{
    Str where = _host + " | resCGI";

    List path; 
    path.push_back(_route->_root + _path + _file);

    Header env;
    env["REQUEST_METHOD"] = _method;
    env["CONTENT_LENGTH"] = toStr(_contentLen);
    env["CONTENT_TYPE"]   = _contentType;
    env["PATH_INFO"]      = _route->_root + _path + _file;

    CGIHandler handler(env, _data, path);

    try
    { 
        _reply = handler.Execute();
        return (logAction(where, msg + ": Executed CGI for " + _path + _file), true); 
    }
    catch(std::exception &e) 
    { 
        resError(_500, msg + ": resCGI: " + Str(e.what() + Str(" >> ") + _path + _file)); 
    }
    return false;
}
