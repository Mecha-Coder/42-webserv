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
    if (x.isURI_noRoute())
        return x.resError(_404, "Route rule: No route for " + x._uri);
    
    if (x.isRedirect_True()) 
        return x.resRedirect();
    
    if (x.isMethod_Illegal())
        return x.resError(_405, "Route rule: Method \"" + x._method + "\" not allowed in " + x._uri);

    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////

bool do_GET(Client &x) 
{
    int type = x.getResourceType();

    if (type == -1) 
        return  x.resError(_404, "Do GET: Resource \"" + x._uri + "\" not found");

    if (type == TYPE_FOLDER) 
    {
        if (x.isURI_noSlash()) 
            return x.resAddSlash();
        
        if (x.noDefaultFile())
        {
            if (x.isAutoIndex_On())
                return x.resDirList();

            return x.resError(_403, "Do GET: No default file or autoindex for " + x._uri);
        }
    }
    
    if (x.isCGI()) 
        return x.resCGI("Do GET");
    
    return x.resFetchFile();
}

/////////////////////////////////////////////////////////////////////////////////////////////

bool do_POST(Client &x) 
{
    int type = x.getResourceType();

    if (type == -1)
        return x.resError(_404, "Do POST: Resource \"" + x._uri + "\" not found");

    if (type == TYPE_FOLDER)
    {
        if (x.isURI_noSlash()) 
            return x.resAddSlash();

        if (x.isReq_Upload())
            return x.resSaveFile();
        
        if (x.noDefaultFile())
            return x.resError(_403, "Do POST: Invalid Post Request");
    }

    if (x.isCGI())
        return x.resCGI("Do Post");
    
    return x.resError(_403, "File \"" + x._uri +  "\" not CGI");  
}

/////////////////////////////////////////////////////////////////////////////////////////////

bool do_DELETE(Client &x) 
{

    int type = x.getResourceType();

    if (type == -1)
        return x.resError(_410, "Do Delete: Resource \"" + x._uri + "\" not found");

    if (type == TYPE_FOLDER)
    {
        if (x.isURI_noSlash()) 
            return x.resError(_403, "Do DELETE: uri must send with \"/\" ->" + x._uri);
        
        return x.resDeleteDir();
    }

    return x.resDeleteFile();
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
