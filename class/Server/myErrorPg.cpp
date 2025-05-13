#include "../../include/Server.hpp"

/*
Purpose:
- Determine if server have its own error page for the given status code
- If yes, return the absolute path to that error page (root + subdir)

_______________________________________________________________________________

Return:
- Absolute path string ➡️ Custom error page exists for the code
- Empty string         ➡️ No custom error page defined for the code
*/
const Str Server::myErrorPg(const int &code) const
{
    ErrorPage::const_iterator i = _errorPage.begin();
    for (; i != this->_errorPage.end(); i++)
    {
        if (i->first == code)
            return (_mainRoot + i->second);
    }
    return "";
}