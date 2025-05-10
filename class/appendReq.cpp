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
void parseHead(Client &client, Str s);

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
    if (this->headerReady()== false);
    {
        pos = data.find("\r\n\r\n");
        if (pos == std::string::npos)
            return false;
       
        info = data.substr(0, pos + 2);
        
        pos = info.find("\r\n");
        for (int i = 0; pos != info.npos; i++)
        {
            if (i == 0)  parseLine(*this, info.substr(0, pos));
            else         parseHead(*this, info.substr(0, pos));

            info.erase(0, pos + 2);
            pos = info.find("\r\n");
        }
    }
    return (data.size() >= this->contentLen);
}


void parseLine(Client &client, Str s)
{
    size_t pos;

    pos = s.find(" ");
    if (pos != s.npos)
    {
        client.method = s.substr(0, pos);
        s.erase(pos + 1);
    }

    pos = s.find(" ");
    if (pos != s.npos)
    {
        client.path = s.substr(0, pos);
        s.erase(pos + 1);
    }

    client.version = s;
}

void parseHead(Client &client, Str s)
{
    Str key, value;
    size_t pos;

    pos = s.find(': ');
    if (pos != s.npos)
    {
        key = s.substr(0, pos);
        value = s.substr(pos + 2);
        client.setHeader(key, value);
    }
}