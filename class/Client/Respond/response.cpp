#include "../../include/Client.hpp"

bool readDir(const Str &path, DirItems &items);
bool readFile(const Str& filename, Str &content);

bool saveMultiPart(Str &bodyPart, const Str &saveHere, Str &filename);

bool deleteFile(const Str& filePath);
bool deleteFolder(const Str& path);

/////////////////////////////////////////////////////////////////////////////////////

bool Client::resDefaultError(Code code)
{
    Str where = _host + " | resDefaultError";

    _reply = tmplErrDefault(code);
    return (logError(where, "Respond: " + toStr(code)), false);
}

//////////////////////////////////////////////////////////////////////////////////////

bool Client::resError(Code code)
{
    Str where = _host + " | resError";

    Str path, body;
    
    path = _server.myErrorPg(code);
    if (!path.empty() && readFile(path, body))
    {
        _reply = tmplErrCustom(code, body);
        return (logError(where, "Respond: " + toStr(code)), false);
    }
    return resDefaultError(code);
}

//////////////////////////////////////////////////////////////////////////////////////

bool Client::resDirList()
{
    Str where = _host + " | resDirList";
    
    DirItems items;

    if (readDir(_route->_root + _path, items))
    {
        _reply = tmplDirList(_path, items);
        return (logAction(where, "Respond: 200: Return directory list " + _path), true);
    }
    return (logError(where, "Failed to create directory list"), resError(_500));
}

/////////////////////////////////////////////////////////////////////////////////////

bool Client::resFetchFile()
{   
    Str where = _host + " | resFetchFile";

    Str body;

    if (readFile(_route->_root + _uri, body))
    {
        _reply = tmplFetch(_file, body);
        return (logAction(where, "Respond: 200: Fetch " + _uri), true);
    }
    return (logError(where, "Fail to read " + _uri), resError(_500));
}

/////////////////////////////////////////////////////////////////////////////////////

bool Client::resAddSlash()
{
    Str where = _host + " | resAddSlash";

    _reply = tmplRedirect(_308, _uri + "/");
    return (logAction(where, "Respond: 308"), false);
}

/////////////////////////////////////////////////////////////////////////////////////

bool Client::resRedirect()
{
    Str where = _host + " | resRedirect";

    _reply = tmplRedirect(_301, _route->_redirect);
    return (logAction(where, "Respond: 301"), false);
}

/////////////////////////////////////////////////////////////////////////////////////

bool Client::resSaveFile()
{
    Str where = _host + " | resSaveFile";
    
    size_t pos;
    List saveFile;
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
        
        if (saveMultiPart(bodyPart, _route->_uploadDir, filename))
        {
            logAction(where, filename + " saved");
            saveFile.push_back(filename);
        }
        else logError(where, filename + " failed to save");
    }

    if (saveFile.empty())
        return resError(_500);

    _reply = tmplSave(saveFile);
    return (logAction(where, "Respond: 201"), true);
}

/////////////////////////////////////////////////////////////////////////////////////

bool Client::resDeleteFile()
{
    Str where = _host + " | resDeleteFile";

    if (deleteFile(_route->_root + _uri))
    {
        _reply = tmplDelete(_uri);
        return (logAction(where, "Respond: 204: Deleted " + _uri), true);
    }
    return (logError(where, "Failed to delete " + _uri), resError(_500));
}

/////////////////////////////////////////////////////////////////////////////////////

bool Client::resDeleteDir()
{
    Str where = _host + " | resDeleteDir";

    if (deleteFolder(_route->_root + _path))
    {
        _reply = tmplDelete(_path);
        return (logAction(where, "Respond: 204: Deleted folder" + _path), true);
    }
    return (logError(where, "Failed to delete folder " + _path), resError(_500));
}