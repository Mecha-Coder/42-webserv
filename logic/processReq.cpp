#include "../include/webserv.hpp"

bool validateRequest(Client &x)
{
    Str where = x._host + " | validateRequest"; 

    if (x.isRequestLine_Malform()) return (
        logError(where, "Malform Request line"), x.resError(_400));
    
    if (x._method == "POST")
    {   
        if (x.isContentHeader_Invalid()) return (
            logError(where, "Invalid content header"), x.resError(_400));
        
        if (x.isBody_ExceedLimit()) return (
            logError(where, "Client body exceeded limit"), x.resError(_413));
    }
    return true;
}

bool routeRule(Client &x)
{
    Str where = x._host + " | routeRule"; 

    if (x.isPath_noSlash()) return (
        logAction(where, "Redirect add slash \"/\" to " + x._path), x.resAddSlash());
    
    if (x.isPath_noRoute()) return (
        logError(where, "No route for " + x._path), x.resError(_404));
    
    if (x.isRedirect_True()) return (
        logAction(where, "Redirect to " + x.getRedirect()), x.resRedirect());
    
    if (x.isPath_noExist()) return (
        logError(where  , "Folder \"" + x._path + "\" doesn't exist"), x.resError(_404));
    
    if (!x.isMethod_Illegal()) return (
        logError(where, "Method \"" + x._method + "\" is not allowed"), x.resError(_405));

    return true;
}

bool do_GET(Client &x) 
{
    Str where = x._host + " | GET"; 

    if (x.isFile_Empty() && x.noDefaultFile()) 
    {
        if (x.isAutoIndex_On()) return (
            logAction(where, "Return directory list for" + x._path), x.resDirList());
        
        return (logError(where, "No default file or autoindex for " + x._path), x.resError(_403));
    }

    if (x.isFile_noExist()) return (
        logMsg(where, "File \"" + x._uri + "\"not found", 0), x.resError(_404));
    
    if (x.isCGI()) return ( 
        logAction(where, "Run CGI on " + x._uri), x.handleCGI());
    
    return (logAction(where, "Fetch " + x._uri), x.resFetchFile());
}

void do_POST(Client &client) 
{
    Str where = client.getHost() + " | POST";

    if (!client._uploadDir.empty() 
        && client._contentType.find("multipart/form-data; boundary=") ==  client._contentType.npos)
    {
        logMsg(where, "Upload file to " + client._uploadDir, 1);
        client.resSaveFile();
    }
    else
    {
        if (client._file.empty())
        {
            client._file = client.getDefaultFile();
            if (client._file.empty() || !isValidFile(client._filePath + client._file))
            {
                logMsg(where, "Invalid Post Request", 0);
                client.resError(403); return;
            }
        }
        else
        {
            if (!isValidFile(client._filePath + client._file))
            {
                logMsg(where, "File \"" + client._path + client._file + "\"not found ", 0);
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
            logMsg(where, "File \"" + client._path + client._file +  "\"not CGI", 0);
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
            logMsg(where, "Delete folder " + client._path, 1);
            client.resDeleteDir(); return;
        }
        if (!isValidFile(client._filePath + client._file))
        {
            logMsg(where, "Is CGI, but file \"" + client._path + client._file + "\"not found", 0);
            client.resError(403); return;
        }
    }
    else
    {
        if (!isValidFile(client._filePath + client._file))
        {
            logMsg(where, "File \"" + client._path + client._file + "\"not found ", 0);
            client.resError(410); return;
        }
        if (!client.isCGI())
        {
            logMsg(where, "Delete file \"" + client._path + client._file + "\"", 1);
            client.resDeleteFile(); return;
        }
    }

    logMsg(where, "Run CGI on " + client._path + client._file, 1);
    client.handleCGI();
}

void processReq(Client &client)
{
    Str where = client._host + " | processReq";

    if (!validateRequest(client) || !routeRule(client))
        return;
    else if (client._method == "GET")    do_GET(client);
    else if (client._method == "POST")   do_POST(client);
    else if (client._method == "DELETE") do_DELETE(client);
    else
    {
        logMsg(where, "Fail to process " + client._path, 0);
        client.resError(_500);
    }
}
