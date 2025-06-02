#include <stdio.h>

int main(void) 
{
    printf("Content-Type: text/html; charset=utf-8\r\n\r\n");
    printf("<html><body><h1>Hello from C CGI!</h1></body></html>\n");
    return 0;
}
