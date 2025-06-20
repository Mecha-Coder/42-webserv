#pragma once

#include "Table.hpp"
using namespace std;

namespace toml {
Table* ParseStream(ifstream&);
Table* ParseFile(std::string&);
}
