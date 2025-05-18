#include "client_tmp.hpp"

Client::Client()
{
}

Client::~Client()
{
}

Client::Client(const Client& other)
{
    *this = other;
}

Client& Client::operator=(const Client& other)
{
    if (this == &other)
        return *this;
    return *this;
}

std::string Client::get_path(int which) const
{
    if (which == 1)
        return (this->_default_path);
    else if (which == 2)
        return (this->_file_path);
    else if (which == 3)
        return (this->_upload_path);
    return "";
}