#include "../../include/Client.hpp"

void parseLine(Client &client, Str line);
void parseHead(Client &client, Header &header, Str head);
void getRouteInfo(Client &client, Route *route);
/* 
Purpose:
- Store the HTTP request string received from recv().
- Accumulate the data until a complete request is received.
- Return TRUE when the request is ready for processing.

_______________________________________________________________________________

Logic:

Header not yet parsed
*********************
- Check if the accumulated string contains "\r\n\r\n":
    - If not found, return FALSE (request incomplete).
    - If found, separate and remove the headers from the accumulator.
    - The remaining data in the accumulator is the HTTP body.
    - Parse the headers and store relevant information (e.g., Content-Length).
-       👇
Header already parsed
*********************
- Check if the size of the accumulated >= Content-Length:
    - If yes, return TRUE (request complete).
    - Otherwise, return FALSE (still waiting for more data).

_________________________________________________________________________________

Return:
    TRUE  ➡️ Full request received and ready to process.
    FALSE ➡️ Request is incomplete; continue accumulating.
*/
bool Client::appendReq(Str request)
{
    Str     info;
    size_t  pos;
    
    this->data += request;
    
    if (this->isHeadReady() == false)
    {
        pos = data.find("\r\n\r\n");
        if (pos == std::string::npos)
            return false;
       
        info = data.substr(0, pos + 2);
        data.erase(0, pos + 4);
    
        pos = info.find("\r\n");
        for (int i = 0; pos != info.npos; i++)
        {
            if (i == 0) parseLine(*this, info.substr(0, pos));
            else        parseHead(*this, this->header, info.substr(0, pos));

            info.erase(0, pos + 2);
            pos = info.find("\r\n");
        }

        this->route = this->server.findRoute(this->_path);
        if (this->route) getRouteInfo(*this, this->route);
    }
    return (data.size() >= this->_contentLen);
}


void getRouteInfo(Client &client, Route *route)
{
    client._redirect = route->_redirect;
    client._filePath = route->_root + route->_uri;
    
    if (!route->_uploadDir.empty())
        client._uploadDir = route->_root + route->_uploadDir;
}


void splitPathFile(Client &client, Str path)
{
    size_t dot = path.rfind(".");
    size_t slash = path.rfind("/");

    if (dot != path.npos && dot > slash)
    {
        client._file = path.substr(slash + 1);
        path.erase(slash + 1);
    } 
    client._path = path;
}


void parseLine(Client &client, Str line)
{
    size_t pos = line.find(" ");

    for (int i = 0; pos != line.npos; i++)
    {
        if      (i == 0)  client._method = line.substr(0, pos);
        else if (i == 1)  splitPathFile(client, line.substr(0, pos));
        
        line.erase(0, pos + 1);
        pos = line.find(" ");
    }
    client._version = line;
}


void parseHead(Client &client, Header &header, Str head)
{
    size_t pos = head.find(": ");

    if (pos != head.npos)
    {
        Str key = head.substr(0, pos);
        Str value = head.substr(pos + 2);
        header[key] = value;

        if (!client._contentLen && key == "Content-Length")
        {
            std::stringstream ss(value);
            ss >> client._contentLen;
        } 
        if (client._contentType.empty() && key == "Content-Type")
            client._contentType = value;
    }
}
