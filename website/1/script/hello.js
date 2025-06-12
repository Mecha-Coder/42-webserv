#!/usr/bin/env node

const body = `
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Hello from Node.js</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background: #f5f5f5;
            color: #333;
            margin: 50px;
            text-align: center;
        }
        h1 {
            color: #2c3e50;
            font-size: 2.5em;
        }
        .box {
            background: white;
            padding: 40px;
            border-radius: 12px;
            box-shadow: 0 0 20px rgba(0,0,0,0.1);
            display: inline-block;
        }
    </style>
</head>
<body>
    <div class="box">
        <h1>Hi from Node.js</h1>
    </div>
</body>
</html>`;

const contentLength = Buffer.byteLength(body);

const headers =
    "HTTP/1.1 200 OK\r\n" +
    "Content-Type: text/html\r\n" +
    `Content-Length: ${contentLength}\r\n\r\n`;

// Output to stdout
process.stdout.write(headers);
process.stdout.write(body);