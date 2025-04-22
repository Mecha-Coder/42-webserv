#include "../include/header.hpp"

bool err_msg(str const &msg, int code)
{
    str Msg = " => " + msg;
    str Code = " => " + std::string(strerror(code));

    std::cerr << RED "Error "  RESET
         << (msg.empty()?   "" : Msg)
         << (code?        Code : "") 
         << "\n";
    return false;
}
