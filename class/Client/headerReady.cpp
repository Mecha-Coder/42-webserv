#include "../../include/Client.hpp"

bool Client::headderReady()
{return (header.size() && !method.empty());}