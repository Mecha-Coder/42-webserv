#pragma once

#include "Table.hpp"
using namespace std;

namespace toml {
Table* parse_stream(ifstream&);
Table* parse_file(std::string&);
}
