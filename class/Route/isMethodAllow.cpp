#include "../../include/Route.hpp"

/* 
Purpose:
- Check if method is permitted for the current route
- If match found in the list of allowed methods and returns TRUE

_______________________________________________________________________________

Return:
    TRUE  ➡️ Method is allowed
    FALSE ➡️ Method is not allowed.
*/
bool Route::isMethodAllow(const Str &method) const
{
    Method::const_iterator i = this->_methods.begin();
    for (; i != this->_methods.end(); i++)
    {
        if (*i == method)
            return true;
    }
    return false;
}
