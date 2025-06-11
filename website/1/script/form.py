#!/usr/bin/env python3

import cgi
import html
import sys
import os
import datetime

#============================================================================
def parse_form() -> dict:

    form = cgi.FieldStorage()
    return {
        'name':        html.escape(form.getfirst("name",        "")),
        'occupation':  html.escape(form.getfirst("occupation",  "")),
        'nationality': html.escape(form.getfirst("nationality", "")),
        'about':       html.escape(form.getfirst("about",       ""))
    }

#============================================================================
def form_summary(data: dict) -> str:

    body = [
        "<!DOCTYPE html>\n",
        "<html lang=\"en\">\n",
        "<head>\n",
        "  <meta charset=\"UTF-8\">\n",
        "  <title>User Information Form</title>\n",
        "  <link href=\"https://fonts.googleapis.com/css2?family=Roboto&display=swap\" rel=\"stylesheet\">\n",
        "  <style>\n",
        "   body {\n",
        "    font-family: 'Roboto', sans-serif;\n",
        "    background-color: #f9f9f9;\n",
        "    margin: 0;\n",
        "    min-height: 100vh;\n",
        "    position: relative;\n",
        "   }\n\n",
        "   .top-header {\n",
        "    position: absolute;\n",
        "    top: 20px;\n",
        "    left: 20px;\n",
        "   }\n\n",
        "   .top-header img.logo-42 {\n",
        "    height: 60px;\n",
        "    width: auto;\n",
        "   }\n\n",
        "   .centering {\n",
        "    display: flex;\n",
        "    align-items: center;\n",
        "    justify-content: center;\n",
        "    min-height: 100vh;\n",
        "   }\n\n",
        "   .form-container {\n",
        "    background-color: #fff;\n",
        "    padding: 30px 40px;\n",
        "    border-radius: 10px;\n",
        "    box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);\n",
        "    width: 100%;\n",
        "    max-width: 500px;\n",
        "   }\n\n",
        "   .form-row {\n",
        "       display: flex;\n",
        "       justify-content: space-between;\n",
        "       align-items: center;\n",
        "       margin-bottom: 20px;\n",
        "   }\n\n",
        "   .form-row label {\n",
        "       font-weight: bold;\n",
        "       color: #555;\n",
        "       margin-right: 10px;\n",
        "       flex: 1;\n",
        "   }\n\n",
        "   .form-row p {\n",
        "       flex: 2;\n",
        "       padding: 10px 12px;\n",
        "       background-color: #f0f0f0;\n",
        "       border-radius: 6px;\n",
        "       margin: 0;\n",
        "   }\n",
        "   h1 {\n",
        "    text-align: center;\n",
        "    margin-bottom: 30px;\n",
        "    color: #333;\n",
        "   }\n\n",
        "   label {\n",
        "    font-weight: bold;\n",
        "    display: block;\n",
        "    color: #555;\n",
        "   }\n\n",
        "   p {\n",
        "    margin-top: 5px;\n",
        "    margin-bottom: 20px;\n",
        "    padding: 0px 12px;\n",
        "   }\n",
        "   button {"
        "   width: 100%;"
        " padding: 12px;"
        " margin-botton: 12px;"
        " font-size: 16px;"
        " background-color: #0066cc;"
        " color: white;"
        " border: none;"
        " border-radius: 6px;"
        " cursor: pointer;"
        " transition: background-color 0.3s ease;"
        " }"
        " button:hover {"
        " background-color: #0052a3;"
        " }"
        ".button-row {"
        "   display: flex;"
        "   justify-content: center;"
        "   gap: 20px;"
        "   margin-top: 30px;"
        "}"
        ".button-row button {"
        "  width: auto;"
        "  min-width: 150px;"
        "  padding: 12px 20px;"
        "}"
        "  </style>\n",
        "</head>\n",
        "<body>\n",
        "  <div class=\"top-header\">\n",
        "    <img class=\"logo-42\" src=\"../photos/42_logo.png\" alt=\"42 Logo\">\n",
        "  </div>\n",
        "  <div class=\"centering\">\n",
        "    <div class=\"form-container\">\n"
    ]

    if not data:
        body.append("      <h1 style=\"color:red\">Fail to save form</h1>\n\n")
    else:
        body.extend([
            "      <h1>User Information Saved</h1>\n\n",
            "      <div class=\"form-row\">\n",
            "        <label for=\"name\">Name:</label>\n",
            f"        <p>{data['name']}</p>\n",
            "      </div>\n\n",

            "      <div class=\"form-row\">\n",
            "        <label for=\"occupation\">Occupation:</label>\n",
            f"        <p>{data['occupation']}</p>\n",
            "      </div>\n\n",

            "      <div class=\"form-row\">\n",
            "        <label for=\"nationality\">Nationality:</label>\n",
            f"        <p>{data['nationality']}</p>\n",
            "      </div>\n\n",

            "      <div class=\"form-row\">\n",
            "        <label for=\"about\">About you:</label>\n",
            f"        <p>{data['about']}</p>\n",
            "      </div>\n\n",
        ])

    body.extend([
        "<div class=\"button-row\">",
        "    <button onclick=\"window.location.href='/archive/forms/';\">See Collection</button>",
        "    <button onclick=\"window.location.href='/form.html';\">Create Another Form</button>",
        "</div>",
        "    </div>\n",
        "  </div>\n",
        "</body>\n",
        "</html>\n"
    ])

    return ''.join(body)

#============================================================================
def make_header(content_length: int) -> str:

    return (
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        f"Content-Length: {content_length}\r\n\r\n"
    )

#============================================================================

def save_form(data: dict, root: str) -> bool:

    timestamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    filename = f"{timestamp}.txt"

    filepath = os.path.join(root, "archive", "forms", filename)


    try:
        with open(filepath, "w", encoding="utf-8") as f:
            for key, value in data.items():
                f.write(f"{key}: {value}\n")
        return True
    except Exception as e:
        print(f"{e}", file=sys.stderr)
        return False

#============================================================================
def main():
    method = os.environ.get("REQUEST_METHOD", "")
    url = os.environ.get("Referer", "")
    root = os.environ.get("ROOT", "")

    if method == "POST" and url.endswith("form.html"):
        data      = parse_form()

        if save_form(data, root):
            body_str  = form_summary(data)
        else:
            body_str  = form_summary(data.clear())

        header_str = make_header(len(body_str.encode()))
        sys.stdout.write(header_str)
        sys.stdout.write(body_str)

#============================================================================

if __name__ == "__main__":
    main()
