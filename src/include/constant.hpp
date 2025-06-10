#ifndef CONSTANT_HPP
#define CONSTANT_HPP

#define IP    "127.0.0.1"
#define BUFFER 1048576  // 1 MB

/****************************************/
// Printing Log Message
/****************************************/

#define YELLOW  "\033[33m"
#define CYAN    "\033[0;36m"
#define GREEN   "\033[0;32m"
#define RED     "\033[0;31m"
#define RESET   "\033[0m"

/****************************************/
// Resource Type
/****************************************/

# define TYPE_FOLDER 0
# define TYPE_FILE 1

/****************************************/
// Status Code
/*****************************************/

typedef enum t_code
{
    _200 = 200, 
    _201 = 201, 
    _204 = 204, 
    _301 = 301, 
    _308 = 308,
    
    _400 = 400,
    _403 = 403,
    _404 = 404, 
    _405 = 405, 
    _410 = 410, 
    _413 = 413,
    _500 = 500 
}   Code;

#endif