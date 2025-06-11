#!/usr/bin/env python3

import os
import sys

#============================================================================

def makeBody(method: str) -> str:

    root = os.environ.get("ROOT", "")

    body = [
        "<!DOCTYPE html>\n",
        "<html><head><title>HTTP Request Detail</title></head>\n",
        "<body>\n",
        "   <h1>From Python CGI</h1>\n"
        "   <h2>HTTP Request Header</h2>\n",
        "   <p>\n",
            os.path.join(root, "archive", "form"),
        "   </p>\n",
        "   <ul>\n"
    ]
    for key, value in os.environ.items():
        body.append(f"       <li><strong>{key}</strong>: {value}</li>\n")
    body.append("   </ul>\n")

    if method == "POST":
        body.extend([
            "   <h2>Here is the body</h2>\n",
            "   <p>\n",
            sys.stdin.read(),
            "   </p>\n",
        ])

    body.append("</body>\n</html>\n")
    return ''.join(body)

#============================================================================

def makeHeader(content_length: int) -> str:
    return (
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        f"Content-Length: {content_length}\r\n\r\n"
    )

#============================================================================

def main():
    method = os.environ.get("REQUEST_METHOD", "")
    body_str = makeBody(method)
    header_str = makeHeader(len(body_str.encode()))

    sys.stdout.write(header_str)
    sys.stdout.write(body_str)

#============================================================================

if __name__ == "__main__":
    main()