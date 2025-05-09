#include "../../include/Client.hpp"

bool Client::headerReady()
{return (header.size() && !method.empty());}