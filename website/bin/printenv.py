import os
import sys

def main():
    # Build HTML body
    body = [
        "<!DOCTYPE html>",
        "<html><head><title>HTTP Request Details</title></head><body>",
        "<h1>HTTP Request Details</h1>",
        "<ul>"
    ]
    
    # Add environment variables
    for key, value in os.environ.items():
        body.append(f"<li><strong>{key}</strong>: {value}</li>")
    
    body.append("</ul></body></html>")
    body_str = "\n".join(body)
    
    # Build headers
    headers = [
        "Content-Type: text/html",
        f"Content-Length: {len(body_str.encode())}",  # Proper byte count
        ""  # Empty line to separate headers and body
    ]
    headers_str = "\r\n".join(headers)
    
    # Output response
    sys.stdout.write(headers_str)
    sys.stdout.write(body_str)

if __name__ == "__main__":
    main()