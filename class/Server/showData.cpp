#include "../../include/Server.hpp"

void Server::showData() const
{
    std::cout << "Server data for " << _serverName << std::endl;
    std::cout << "=====================================\n"
              << "Main Root    : [" << this->_mainRoot << "]\n"
              << "Cliet Max    : [" << this->_clientBodySize << "]\n"
              << "Listen       : " << std::endl;
    
    Address::const_iterator i = _listen.begin();
    for (; i != this->_listen.end(); i++)
        std::cout << "\t" << *i << std::endl;
    
    std::cout << "\nError Pg : " << std::endl;
    
    ErrorPage::const_iterator j = _errorPage.begin();
    for (; j != this->_errorPage.end(); j++)
        std::cout << "\t" << j->first << ", " << j->second << std::endl;
    std::cout << std::endl;

    Routes::const_iterator k = _routes.begin();
    for (; k != this->_routes.end(); k++)
        k->showData();
}

