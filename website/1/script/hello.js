#!/usr/bin/env node

const body = `
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Hello from Node.js</title>
<style>
    body {
        margin: 0;
        padding: 0;
        height: 100vh;
        display: flex;
        justify-content: center;
        align-items: center;
        font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
        background: linear-gradient(135deg, #f0f0f0, #dfe9f3);
        color: #333;
    }

    h1 {
        color: #2c3e50;
        font-size: 2.5em;
        margin: 0;
    }

    .box {
        background: white;
        padding: 50px;
        border-radius: 16px;
        box-shadow: 0 10px 30px rgba(0, 0, 0, 0.15);
        transition: transform 0.3s ease, box-shadow 0.3s ease;
    }

    .box:hover {
        transform: translateY(-5px);
        box-shadow: 0 15px 40px rgba(0, 0, 0, 0.2);
    }

    .top-header {
        position: absolute;
        top: 20px;
        left: 20px;
    }

    .top-header img.logo-42 {
        height: 60px;
        width: auto;
    }
</style>
</head>
<body>
    <div class=\"top-header\">
        <a href=\"/\">
        <img class=\"logo-42\" src=\"../photos/42_logo.png\" alt=\"42 Logo\">
        </a>
    </div>
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