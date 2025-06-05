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
    
    if (x.isMethod_Illegal())
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
        return  x.resError(_404, "Do GET: File \"" + x._path + x._file + "\" not found");
    
    if (x.isCGI()) 
        return x.resCGI("Do GET");
    
    return x.resFetchFile();
}

/////////////////////////////////////////////////////////////////////////////////////////////

bool do_POST(Client &x) 
{
    if (x.isReq_Upload())
        return x.resSaveFile();
        
    if (x.isFile_Empty() && x.noDefaultFile())
        return x.resError(_403, "Do POST: Invalid Post Request");
    
    if (x.isFile_noExist())
        return x.resError(_404, "Do POST: File \"" + x._path + x._file + "\" not found");

    if (x.isCGI())
        return x.resCGI("Do Post");
    
    return x.resError(_403, "File \"" + x._path + x._file +  "\" not CGI");  
}

/////////////////////////////////////////////////////////////////////////////////////////////

bool do_DELETE(Client &x) 
{
    if (x.isFile_Empty())
    {
        if (x.noDefaultFile())
            return x.resDeleteDir();
        
        if (x.isFile_noExist() || !x.isCGI())
            return x.resError(_403, "Do Delete: Invalid Delete Request");
    }
    else
    {
        if (x.isFile_noExist())
            return x.resError(_410, "Do Delete: File \"" + x._path + x._file + "\" not found");

        if (!x.isCGI())
            return x.resDeleteFile();
    }
    return x.resCGI("Do Delete");
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
        client.resError(_500, "Process req: Fail to process");
}
