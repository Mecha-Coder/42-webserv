#include "../include/webserv.hpp"

bool validateRequest(Client &client)
{
    Str where = client.getHost() + " | validateRequest"; 

    if (client._method.empty() || client._path.empty() || client._version != "HTTP/1.1")
    {
        logMsg(where, "400: Malform Request line", 0);
        return (client.resError(400), false);
    }     
    if (client._method == "POST")
    {   
        if (client._contentType.empty() || client._contentLen == 0 || !client.isBodyMatchLen())
        {
            logMsg(where, "400: Invalid content header", 0);
            return (client.resError(400), false);
        }
        if (!client.isBodyWithinLimit())
        {
            logMsg(where, "413: Client body exceed limit", 0);
            return (client.resError(413), false);
        }
    }
    return true;
}

bool routeRule(Client &client)
{
    Str where = client.getHost() + " | routeRule"; 

    if (*client._path.rbegin() != '/')
    {
        logMsg(where, "308: Redirect add slash '/'", 1);
        return (client.resRedirectAddSlash(), false);
    }
    if (!client.haveRoute())
    {
        logMsg(where, "404: No route " + client._path, 0);
        return (client.resError(404), false);
    }
    if (!client._redirect.empty())
    {
        logMsg(where, "301: Redirect to " + client._redirect, 1);
        return (client.resRedirectTo(), false);
    }
    if (!isFolderExist(client._filePath))
    {
        logMsg(where, "404: Folder doesn't exist " + client._filePath, 0);
        return (client.resError(404), false);
    }
    else if (!client.isMethodAllow())
    {
        logMsg(where, "405: Method not allowed " + client._method, 0);
        return (client.resError(405), false);
    }
    return true;
}

void do_GET(Client &client) 
{
    Str where = client.getHost() + " | GET"; 

    if (client._file.empty())
        client._file = client.getDefaultFile();
    if (client._file.empty())
    {
        if (client.isAutoIndex())
        {
            logMsg(where, "200: Return directory list", 1);
            client.resDirList();
        }
        else
        {
            logMsg(where, "403: No default file, no autoindex", 0);
            client.resError(403);
        }
    }
    else
    {
        if (isValidFile(client._filePath + client._file))
        {
            if (client.isCGI()) 
            {
                logMsg(where, "Run CGI on " + client._path + client._file, 1);
                client.handleCGI();
            }
            else
            {
                logMsg(where, "200: Retrieve " + client._path + client._file, 1);
                client.resFectchFile();
            }
        }
        else
        {
            logMsg(where, "404: File not found " + client._path + client._file, 0);
            client.resError(404);
        }
    }
}

void do_POST(Client &client) 
{
    Str where = client.getHost() + " | POST";

    if (!client._uploadDir.empty())
    {
        logMsg(where, "201: Save file @ " + client._uploadDir, 1);
        client.resSaveFile();
    }
    else
    {
        if (client._file.empty())
        {
            client._file = client.getDefaultFile();
            if (client._file.empty() || !isValidFile(client._filePath + client._file))
            {
                logMsg(where, "403: Invalid Post Request", 0);
                client.resError(403); return;
            }
        }
        else
        {
            if (!isValidFile(client._filePath + client._file))
            {
                logMsg(where, "404: File not found " + client._path + client._file, 0);
                client.resError(404); return;
            }
        }

        if (client.isCGI())
        {
            logMsg(where, "Run CGI on " + client._path + client._file, 1);
            client.handleCGI();
        }
        else
        {
            logMsg(where, "403: File not CGI " + client._path + client._file, 0);
            client.resError(403);
        }
    }    
}

void do_DELETE(Client &client) 
{
    Str where = client.getHost() + " | DELETE";

    if (client._file.empty())
    {
        client._file = client.getDefaultFile();
        if (client._file.empty() || !client.isCGI())
        {
            logMsg(where, "204: Delete folder " + client._path, 1);
            client.resDeleteDir(); return;
        }
        if (!isValidFile(client._filePath + client._file))
        {
            logMsg(where, "403: Is CGI, but file not found " + client._path + client._file, 0);
            client.resError(403); return;
        }
    }
    else
    {
        if (!isValidFile(client._filePath + client._file))
        {
            logMsg(where, "410: File not found " + client._path + client._file, 0);
            client.resError(410); return;
        }
        if (!client.isCGI())
        {
            logMsg(where, "204: Delete file " + client._path + client._file, 1);
            client.resDeleteFile(); return;
        }
    }

    logMsg(where, "Run CGI on " + client._path + client._file, 1);
    client.handleCGI();
}

void processReq(Client &client)
{
    Str where = client.getHost() + " | processReq";

    if (!validateRequest(client) || !routeRule(client))
        return;
    else if (client._method == "GET")    do_GET(client);
    else if (client._method == "POST")   do_POST(client);
    else if (client._method == "DELETE") do_DELETE(client);
    else
    {
        logMsg(where, "500: Fail to process " + client._path, 0);
        client.resError(500);
    }
}
