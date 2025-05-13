/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   appendReq.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaul <jpaul@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:27:03 by jpaul             #+#    #+#             */
/*   Updated: 2025/05/09 21:49:14 by jpaul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Client.hpp"

void parseLine(Client &client, Str s);
void parseHead(Client &client, Header &header, Str s);
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
bool Client::appendReq(char &request)
{
    Str     info;
    size_t  pos;
    
    this->data += request;
    if (this->isHeadReady() == false);
    {
        pos = data.find("\r\n\r\n");
        if (pos == std::string::npos)
            return false;
       
        info = data.substr(0, pos + 2);
        data.erase(0, pos + 4);
        
        pos = info.find("\r\n");
        for (int i = 0; pos != info.npos; i++)
        {
            if (i == 0)  parseLine(*this, info.substr(0, pos));
            else         parseHead(*this, this->header, info.substr(0, pos));

            info.erase(0, pos + 2);
            pos = info.find("\r\n");
        }

        this->route = this->server.findRoute(this->_path);
        if (this->route) getRouteInfo(*this, this->route);

        std::cout << "HTTP header successfully parse" << std::endl;
    }
    return (data.size() >= this->_contentLen);
}

void getRouteInfo(Client &client, Route *route)
{
    client._redirect = route->_redirect;
    client._filePath = route->_root + route->_uri;
    client._uploadDir = route->_uploadDir;
}

void splitPathFile(Client &client, Str s)
{
    size_t dot = s.rfind(s);
    size_t slash = s.rfind(s);

    if (dot != s.npos && dot > slash)
    {
        client._file = s.substr(slash + 1);
        s.erase(slash + 1);
    } 
    client._path = s;
}

void parseLine(Client &client, Str s)
{
    size_t pos = s.find(" ");
    for (int i = 0; pos != s.npos; i++)
    {
        if      (i == 0)   client._method = s.substr(0, pos);
        else if (i == 1)   splitPathFile(client, s.substr(0, pos));
        
        s.erase(pos + 1);
        pos = s.find(" ");
    }
    client._version = s;
}

void parseHead(Client &client, Header &header, Str s)
{
    size_t pos = s.find(': ');

    if (pos != s.npos)
    {
        Str key = s.substr(0, pos);
        Str value = s.substr(pos + 2);
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