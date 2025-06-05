#!/usr/bin/env python3

import sys

def main():
    # Build the HTML body
    body = [
        "<!DOCTYPE html>\n",
        "<html><head><title>HTTP Request Body</title></head><body>\n",
        "<h1>Here is the body</h1><p>\n",
        sys.stdin.read(),
        "</p></body></html>\n"
    ]
    body_str = ''.join(body)
    content_length = len(body_str)
    
    # Build headers
    headers = [
        "Content-Type: text/html",
        f"Content-Length: {content_length}",
        ""  # Blank line to separate headers and body
    ]
    headers_str = "\r\n".join(headers)
    
    # Output headers and body
    sys.stdout.write(headers_str)
    sys.stdout.write(body_str)

if __name__ == "__main__":
    main()