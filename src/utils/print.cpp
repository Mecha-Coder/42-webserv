#include "../include/webserv.hpp"

/////////////////////////////////////////////////////////////////////////

Str printmyList(const List &list)
{
    Str s;

    for (size_t i = 0; i < list.size(); i++)
    {
        s += list[i];
        if ((i + 1) != list.size())
            s += ", ";
    }
    return s;
}

/////////////////////////////////////////////////////////////////////////

Str printmyMap(const ErrorPage &map)
{
    Str s;

    ErrorPage::const_iterator i = map.begin();
    for (; i != map.end(); i++)
        s += Str("\t\t[") + toStr(i->first) + Str("] => [") + i->second + "]\n";

    return s;
}