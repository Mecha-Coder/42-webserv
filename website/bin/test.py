import os

def main():
    respondBody = []
    respondBody.append("<!DOCTYPE html>\n")
    respondBody.append("<html><head><title>HTTP Request Details</title></head><body>")
    respondBody.append("<h1>HTTP Request Details</h1>\n")
    respondBody.append("<ul>\n")

    for key, value in os.environ.items():
        respondBody.append(f"<li><strong>{key}</strong>: {value}</li>\n")

    respondBody.append("</ul></body></html>\n")

    response_content = ''.join(respondBody)

    print("Content-Type: text/html\r\n")
    print(f"Content-Length: {len(response_content)}\r\n")

    print(response_content)

if __name__ == "__main__":
    main()