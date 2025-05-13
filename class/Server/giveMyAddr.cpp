#include "../../include/Server.hpp"

/*
Purpose:
- Fills the server's address in the provided list (passed a param) as an IP-host pair
- The method is responsible to spilt address into "IP" and "port" components
- The list is used for creating listening sockets

_______________________________________________________________________________

Return:
- No return
*/
void Server::giveMyAddr(IP_Host &addrList) const
{
    size_t pos;
    Str IP, Port;

    Address::const_iterator i = _listen.begin();
    for (; i != _listen.end() ; i++)
    {
        pos  = i->find(":");
        IP   = i->substr(0, pos);
        Port = i->substr(pos + 1);
        addrList.push_back(std::make_pair(IP, Port));
    }
}
