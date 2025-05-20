#include "../include/webserv.hpp"

bool validateRequest(Client &client)
{
    if (client._method.empty() || client._path.empty() || client._version != "HTTP/1.1")
    {
        std::cout << RED "Process Request=> Malform request line" RESET << std::endl;
        client.resError(400);
        return false; 
    }     
    else if (client._method == "POST")
    {   
        if (client._contentType.empty() || client._contentLen == 0 || !client.bodySizeMatch())
        {
            std::cout << RED "Process Request=> Invalid content header" RESET << std::endl;
            client.resError(400);
            return false;
        }
        else if (!client.bodyWithinLimit())
        {
            std::cout << RED "Process Request=> Client body exceed limit" RESET << std::endl;
            client.resError(413);
            return false;
        }
    }
    return true;
}

bool routeRule(Client &client)
{
    if (*client._path.rbegin() != '/')
    {
        client.resRedirectAddSlash();
    }
    else if (!client.haveRoute())
    {
        std::cout << RED "Process Request=> No route=> " RESET << client._path << std::endl;
        client.resError(404);
    }
    else if (!client._redirect.empty())
    {
        client.resRedirectTo();
    }
    else if (!isFolderExist(client._filePath))
    {
        std::cout << RED "Process Request=> Path doesn't exist" RESET << std::endl;
        client.resError(404);
    }
    else if (!client.isMethodAllow())
    {
        client.resError(405);
    }
    else return true;
    return false;
}

void do_GET(Client &client) 
{
    std::cout << "Do GET request" << std::endl;

    if (client._file.empty())
        client._file = client.getDefaultFile();
    if (client._file.empty())
    {
        if (client.isAutoIndex())
            client.resDirList();
        else
        {
            std::cout << RED "Process Request=> GET=> No default, no autoindex" RESET << std::endl;
            client.resError(403);
        }
    }
    else
    {
        if (isValidFile(client._filePath + client._file))
        {
            if (client.runWithCGI())  client.handleCGI();
            else                      client.resFectchFile();
        }
        else
        {
            std::cout << RED "Process Request=> GET=> No such file=> " RESET << client._file << std::endl;
            client.resError(404);
        }
    }
}

void do_POST(Client &client) 
{
    std::cout << "Do POST request" << std::endl;
    std::cout << client._uploadDir << std::endl;

    if (client._uploadDir.empty())
    {
        if (client._file.empty())
        {
            client._file = client.getDefaultFile();
            if (client._file.empty() || !isValidFile(client._filePath + client._file))
            {
                std::cout << RED "Process Request=> POST=> Invalid Post Request=> " RESET << std::endl;
                client.resError(403);
                return;
            }
        }
        else
        {
            if (!isValidFile(client._filePath + client._file))
            {
                std::cout << RED "Process Request=> POST=> No such file=> " RESET << client._file << std::endl;
                client.resError(404);
                return;
            }
        }

        if (client.runWithCGI()) client.handleCGI();
        else
        {
            std::cout << RED "Process Request=> POST=> Not a CGI=> " RESET << client._file << std::endl;
            client.resError(403);
        }
    }
    else 
        client.resSaveFile();
}

void do_DELETE(Client &client) 
{
    std::cout << "Do DELETE request" << std::endl;

    if (client._file.empty())
    {
        client._file = client.getDefaultFile();
        std::cout << "Checking>> " << client._file << std::endl;
        if (client._file.empty() || !client.runWithCGI())
        {
            client.resDeleteDir();
            return;
        }
        else
        {
            if (!isValidFile(client._filePath + client._file))
            {
                std::cout << RED "Process Request=> DELETE=> CGI, No file exist=> " RESET << client._file << std::endl;
                client.resError(403);
                return;
            }
        }
    }
    else
    {
        if (!isValidFile(client._filePath + client._file))
        {
            std::cout << RED "Process Request=> DELETE=> File gone=> " RESET << client._file << std::endl;
            client.resError(410);
            return;
        }
        else
        {
            if (!client.runWithCGI())
            {
                client.resDeleteFile();
                return;
            }
        }
    }

    client.handleCGI();
}

void processReq(Client &client)
{
    if (!validateRequest(client) || !routeRule(client))
        return;
    else if (client._method == "GET")    do_GET(client);
    else if (client._method == "POST")   do_POST(client);
    else if (client._method == "DELETE") do_DELETE(client);
    else
    {
        std::cout << RED "Process Request=> Fail to process" RESET << std::endl;
        client.resError(500);
    }
}
