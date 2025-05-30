#include "../include/webserv.hpp"

bool validateRequest(Client &x)
{
    if (x.isRequestLine_Malform())
        return x.resError(_400, "Validate req: Malform request line");
    
    if (x._method == "POST")
    {   
        if (x.isContentHeader_Invalid())
            return x.resError(_400, "Validate req: Invalid content header");
        
        if (x.isBody_ExceedLimit())
            return x.resError(_413, "Validate req: Client body exceeded limit");
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////

bool routeRule(Client &x)
{
    if (x.isPath_noSlash()) 
        return x.resAddSlash();

    if (x.isPath_noRoute())
        return x.resError(_404, "Route rule: No route for " + x._path);
    
    if (x.isRedirect_True()) 
        return x.resRedirect();
    
    if (x.isPath_noExist())
        return x.resError(_404, "Route rule: Folder \"" + x._path + "\" doesn't exist");
    
    if (!x.isMethod_Illegal())
        return x.resError(_405, "Route rule: Method \"" + x._method + "\" is not allowed");

    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////

bool do_GET(Client &x) 
{
    if (x.isFile_Empty() && x.noDefaultFile()) 
    {
        if (x.isAutoIndex_On())
            return x.resDirList();

        return x.resError(_403, "Do GET: No default file or autoindex for " + x._path);
    }

    if (x.isFile_noExist()) 
        return  x.resError(_404, "Do GET: File \"" + x._uri + "\"not found");
    
    if (x.isCGI()) 
        return x.resCGI("Do GET");
    
    return x.resFetchFile();
}

/////////////////////////////////////////////////////////////////////////////////////////////

bool do_POST(Client &x) 
{
    if (x.isReq_Upload())
        return x.resSaveFile();
        
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

/////////////////////////////////////////////////////////////////////////////////////////////

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
