#!/usr/bin/env php
<?php
$body = "
<!DOCTYPE html>
<html lang=\"en\">
<head>
    <meta charset=\"UTF-8\">
    <title>Hello from PHP</title>
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
    <div class=\"box\">
        <h1>Hi from PHP</h1>
    </div>
</body>
</html>";

$contentLength = strlen($body);

$header = "HTTP/1.1 200 OK\r\n" .
		  "Content-Type: text/html\r\n" .
		  "Content-Length: $contentLength\r\n\r\n";

echo $header;
echo $body;
?>