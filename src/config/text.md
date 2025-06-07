TOML File
  │
  ▼
ParseFile("config.toml")
  │
  ▼
ParseStream(ifs)
  │
  ▼
┌────────────────────────┐
│  Lexer + Parser Logic  │
└────────────────────────┘
  │
  ▼
TokenList + SyntaxCheck
  │
  ▼
Build(Parser p)
  │
  ▼
Construct Table Tree (Table*)
  │
  ▼
Config(*Table)


1. ParseFile("config.toml")
std::ifstream ifs(filename.c_str());
return ParseStream(ifs);

➡️ Opens the TOML file and forwards the stream to ParseStream().

2. ParseStream(std::ifstream&)
Lexer lexer(in);
TokenListResult resToken = lexer.Parse();

➡️ Lexing phase:

    Turns the file into a list of tokens (strings, identifiers, symbols).

3. SyntaxChecker(tokens)
ChekerResult res = SyntaxChecker(tokens);

➡️ Checks:

    Are tokens arranged in a valid TOML structure?

✅ If good → construct Parser p(tokens);

4. Build(Parser& p)
Table* t = new Table(Table::TABLE);
FillMap(p.mp, *t);          // Top-level assignments
FOR_EACH(p.tables, it) {    // [table] or [[table]]
    ...
}
return t;

➡️ Creates the root TOML table (Table*)

➡️ Calls FillMap() to add all top-level key-values.

➡️ Iterates over named table blocks ([x] and [[x]]):

    Uses List2Map() to navigate to the right spot.

    Uses FillMap() to populate keys.

5. FillMap(TokenMap&, Table&)
For each TokenMap entry:

[server]
host = "127.0.0.1"

➡️ Calls List2Map(it->key, t) → finds or creates t["server"].

➡️ If is_array, sets:

t["server"].type = ARRAY
t["server"].vec.push_back("value")

Else sets:

t["server"]["host"] = "127.0.0.1"

6. List2Map(TokenList, Table&)
Given:
[server.location]

t["server"] → t1
t1["location"] → t2

➡️ Walks through token path and creates sub-tables as needed.

➡️ If intermediate value is an array ([[table]]), it dives into last.vec.back().

7. Result: You Now Have a Table* That Mirrors the TOML
For example, given TOML:

[server]
host = "127.0.0.1"

[[server.location]]
path = "/"

[[server.location]]
path = "/admin"

You get:

t["server"]["host"] → "127.0.0.1"
t["server"]["location"].vec[0]["path"] → "/"
t["server"]["location"].vec[1]["path"] → "/admin"

8. Config(Table& t) — Your Code Uses This Structure
Config cfg(*t);

➡️ You write logic that looks like:

t["server"].vec → vector of server tables

For each server:
  t["server"][i]["location"].vec → locations

You extract config by navigating the Table tree populated by the parser.

TLDR:
[server]
[[server.location]]
path = "/"
[[server.location]]
path = "/admin"

[server] becomes:

root
└── "server"
    └── "location" = ARRAY

[[server.location]] becomes:

root
└── "server"
    └── "location" = ARRAY
        ├── vec[0] = {"path": "/"}
        └── vec[1] = {"path": "/admin"}