#include "../../include/Route.hpp"

Route::Route(
    const Str &uri,
    const Str &root,
    const Str &redirect,
    const Str &defaultFile,
    const Str &uploadDir,
    const bool &autoIndex,
    const Method &methods,
    const CGI &cgi
)
: _methods(methods), _cgi(cgi), _uri(uri), _root(root),
_redirect(redirect),  _uploadDir(uploadDir), _autoIndex(autoIndex),
_defaultFile(defaultFile) {}