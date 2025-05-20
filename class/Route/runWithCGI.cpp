#include "../../include/Route.hpp"

/*
Purpose:
- Checks if the file for the current route should be handled via CGI.
- If so, returns the absolute path to the CGI binary (root + CGI subdirectory).
- This method will slit the file extension from the filename

_______________________________________________________________________________

Return:
- Absolute path string ➡️ Handled with CGI
- Empty string         ➡️ Not handled with CGI
*/
bool Route::runWithCGI(const Str &file) const
{
    Str extension = file.substr(file.rfind("."));
    CGI::const_iterator i = _cgi.begin();
    for (; i != this->_cgi.end(); i++)
    {
        if (*i == extension)
            return true;
    }
    return false;
}
