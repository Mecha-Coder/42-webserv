#!/usr/bin/env python3

import base64
import cgi
import html
import sys
import os

KEY = "!P5t;2<h?A3K+%S:ePE#H_G^+qkmy|UEOcc9lMy\"iB[4C!uH<BjnAD+?h}8K@9"
ROUNDS = 7
DELIMITER = "*)**)^*"

#============================================================================

def xor_crypt_bytes(data):
    key_bytes = KEY.encode('utf-8')
    return bytes(data[i] ^ key_bytes[i % len(key_bytes)] for i in range(len(data)))

def encrypt(message):
    data = message.encode('utf-8')
    for _ in range(ROUNDS):
        data = xor_crypt_bytes(data)
        data = base64.b64encode(data)
    return data.decode('ascii')

def decrypt(encrypted):
    data = encrypted.encode('ascii')
    data = base64.b64decode(data)
    for i in range(ROUNDS):
        data = xor_crypt_bytes(data)
        if i < ROUNDS - 1:
            data = base64.b64decode(data)
    return data.decode('utf-8')

#============================================================================
def parse_form() -> dict:

    form = cgi.FieldStorage()
    return {
        'username':   html.escape(form.getfirst("username", "")),
        'loginTime':  html.escape(form.getfirst("loginTime", "")),
    }

#============================================================================
def parse_cookie(cookie: str) -> dict:

    secret = decrypt(cookie.split('=', 1)[1])
    parts = secret.split(DELIMITER)

    if len(parts) == 2:
        return {
            "username": parts[0],
            "loginTime": parts[1]
        }
    return {}

#============================================================================

def make_header(content_length: int, set_cookie: dict) -> str:

    header  = [
        "HTTP/1.1 200 OK\r\n",
        "Content-Type: text/html\r\n",
        f"Content-Length: {content_length}\r\n",
    ]

    if not set_cookie:
        header.append("\r\n")
    else:
        secret = set_cookie['username'] + DELIMITER + set_cookie['loginTime']
        secret = encrypt(secret)

        header.extend([
            f"Set-Cookie: secret={secret}",
            "\r\n\r\n"
        ])

    return ''.join(header)

#============================================================================


def make_body(data: dict, method: str) -> str:

    body = [
        "<!DOCTYPE html>\n",
        "<html lang=\"en\">\n",
        "<head>\n",
        "  <meta charset=\"UTF-8\">\n",
        "  <title>Remember User</title>\n",
        "  <style>\n",
        "    body {\n",
        "      font-family: sans-serif;\n",
        "      background: #f4f4f4;\n",
        "      display: flex;\n",
        "      flex-direction: column;\n",
        "      align-items: center;\n",
        "      justify-content: center;\n",
        "      height: 100vh;\n",
        "      margin: 0;\n",
        "    }\n",
        "    h1 {\n",
        "      color: #333;\n",
        "    }\n",
        "    .container {\n",
        "      background: white;\n",
        "      padding: 2em;\n",
        "      border-radius: 8px;\n",
        "      box-shadow: 0 0 10px rgba(0,0,0,0.1);\n",
        "      text-align: center;\n",
        "    }\n",
        "    input[type=\"text\"] {\n",
        "      padding: 0.5em;\n",
        "      margin-top: 1em;\n",
        "      width: 80%;\n",
        "      font-size: 1em;\n",
        "    }\n",
        "    button {\n",
        "      padding: 0.5em 1em;\n",
        "      margin-top: 1em;\n",
        "      font-size: 1em;\n",
        "      background-color: #007bff;\n",
        "      color: white;\n",
        "      border: none;\n",
        "      border-radius: 4px;\n",
        "      cursor: pointer;\n",
        "    }\n",
        "  </style>\n",
        "</head>\n",
        "<body>\n",
        "  <div class=\"container\">\n",
        "    <h1>Remember User</h1>\n",
    ]

    if method=="POST" and data:
        body.extend([
            f"    <h3>Thanks {data['username']}</h3>\n",
            "     <p>I will remember you next time you visit</p>\n"
        ])

    elif method=="GET" and data:
        body.extend([
            f"    <h3>Welcome back {data['username']}</h3>\n",
            f"     <p>Yes, I remembered. We became good friends on {data['loginTime']}</p>\n",
            "       <button onclick=\"deleteUsernameCookie()\">Forget Me</button>\n",
    "\n",
        ])

    elif method=="GET" and not data:
        body.extend([
            "    <p>Please let me know your name so that I can remember you next time you visit</p>\n",
            "    \n",
            "    <form id=\"userForm\" method=\"POST\" action=\"/script/remember.py\">\n",
            "      <input type=\"text\" id=\"username\" name=\"username\" placeholder=\"Enter your name\" required />\n",
            "      <input type=\"hidden\" id=\"loginTime\" name=\"loginTime\" />\n",
            "      <br />\n",
            "      <button type=\"submit\">Remember</button>\n",
            "    </form>\n",
        ])

    body.extend([
        "  </div>\n",
        "\n",
        "  <script>\n",
        "    const form = document.getElementById('userForm');\n",
        "    form.addEventListener('submit', function (e){\n",
        "      const name = document.getElementById('username').value.trim();\n",
        "      if (!name) {\n",
        "        e.preventDefault();\n",
        "        alert(\"Please enter your name.\");\n",
        "        return;\n",
        "      }\n",
        "\n",
        "      const now = new Date();\n",
        "      const yy = now.getFullYear().toString().slice(2);\n",
        "      const mm = String(now.getMonth() + 1).padStart(2, '0');\n",
        "      const dd = String(now.getDate()).padStart(2, '0');\n",
        "      const hh = String(now.getHours()).padStart(2, '0');\n",
        "      const min = String(now.getMinutes()).padStart(2, '0');\n",
        "\n",
        "      const readableTime = `Date=${dd}.${mm}.${yy} Time=${hh}:${min}`;\n",
        "      document.getElementById('loginTime').value = readableTime;\n",
        "    });\n",
        "           ",
        "  function deleteUsernameCookie() {\n",
        "    document.cookie = \"secret=; Path=/script; Expires=Thu, 01 Jan 1970 00:00:00 GMT\";\n",
        "    alert(\"Cookie has been deleted.\");\n",
        "    location.reload(); // optional\n",
        "  }\n",
        "  </script>\n",
        "</body>\n",
        "</html>\n",
    ])

    return ''.join(body)


#============================================================================

def main():
    data = {}
    method = os.environ.get("REQUEST_METHOD", "")
    cookie = os.environ.get("Cookie", "BLANK")
    type = os.environ.get("Content-Type", "") 

    if method == "POST" and type == "application/x-www-form-urlencoded":
        data       = parse_form()
        body_str   = make_body(data, method)
        header_str = make_header(len(body_str.encode()), data)

    elif method == "GET":

        if cookie == "BLANK":
            body_str = make_body(data, method)
        else:
            data      = parse_cookie(cookie)
            body_str  = make_body(data, method)

        header_str = make_header(len(body_str.encode()), data)
    
    sys.stdout.write(header_str)
    sys.stdout.write(body_str)

#============================================================================

if __name__ == "__main__":
    main()