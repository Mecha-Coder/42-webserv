#!/usr/bin/env python3
import sys
import os

print("Content-Type: text/plain")
print("")
print("Hello from CGI script!")
print("Content-Length:", os.environ.get("CONTENT_LENGTH", "0"))
data = sys.stdin.read()
print("Received body:", data)
