#include "../../include/Route.hpp"

void Route::showData() const
{
    std::cout << "\tRoute data: " << _uri << std::endl;
    std::cout << "\t*********************\n"
              << "\tRoot      : [" << this->_root << "]\n"
              << "\tRedirect  : [" << this->_redirect << "]\n"
              << "\tUpload Dir: [" << this->_uploadDir << "]\n"
              << "\tAuto Index: [" << this->_autoIndex << "]\n"
              << "\tDefau File: [" << this->_defaultFile << "]\n"
              << "\tMethod    : ";
    
    Method::const_iterator i = _methods.begin();
    for (; i != this->_methods.end(); i++)
        std::cout << *i << ", ";
    
    std::cout << "\n\tCGI path  : " << std::endl;
    
    CGI::const_iterator j = _cgi.begin();
    for (; j != this->_cgi.end(); j++)
        std::cout << "\t\t" << j->first << ", " << j->second << std::endl;
    std::cout << std::endl; 
}
