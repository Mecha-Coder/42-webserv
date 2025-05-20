#include "../../include/Client.hpp"

/*
Purpose:
- Trigger this method after done responding if keep-alive is true
- Wipe all data, except for server

_______________________________________________________________________________

Return:
- return nothing - 
*/
void Client::reUseFd()
{
    this->data.clear();
    this->header.clear();
    this->route = NULL;
    this->reply.clear();
    this->byteSend = 0;

    this->_method.clear();
    this->_path.clear();
    this->_version.clear();
    this->_contentType.clear();
    this->_contentLen = 0;
    this->_file.clear();
    this->_redirect.clear();
    this->_filePath.clear();
    this->_uploadDir.clear();
}