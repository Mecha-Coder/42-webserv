# 🌐 Webserv 42

## How to test server

### 1) Execute Program

```bash
git clone https://github.com/Mecha-Coder/webserv_42.git Webserv
cd Webserv
make
./webserv
```
### 2) Access the Web Server
Open your favorite browser and go to:

```bash
http://127.0.0.1:4000/
```


## How to run siege without installation

### 1) Download portable siege
```bash
wget https://download.joedog.org/siege/siege-latest.tar.gz
tar -xzf siege-latest.tar.gz
```

### 2) Install using make
```bash
cd siege-*/
make
make install
```

### 3) Check Availability
```bash
~/siege-local/bin/siege --version
```

### 4) Run Test
* Ensure ./webserver program is running
```bash
$HOME/siege-local/bin/siege -c 10000 -r 50 http://127.0.0.1:4000/
```

Note: This step is for testing with 42 campus machine. If you are using your personal computer, use -> sudo apt install siege

### Reference

## GCI
[Intro to Common Gateway Interface](http://www.mnuwer.dbasedeveloper.co.uk/dlearn/web/session01.htm)
[CGI Script](http://www.wijata.com/cgi/cgispec.html#4.0)


## Process Request
[Flowchart](https://github.com/kirwako/webserv-request-flow/blob/main/webserv-request-flow.pdf)


## Our WhiteBoard
[Miro WhiteBoard](https://miro.com/welcomeonboard/dTJHWE9IeTZDTzNacHJFbjRuMDZLZ2FyZ0pNYVBybElna0lkQVdyZHFlNjNheUJUMkY5NkFhTlpldEI0WWtKQzErckUxY3ZMK2xuOVlVL1ZzQmw2Z0dSLzFXdTY5bDE4K2VLK3V2Z09QZlJ0M0Q2TW1wdHRZUHRaQkZqZGtJWHFnbHpza3F6REdEcmNpNEFOMmJXWXBBPT0hdjE=?share_link_id=527232404402)


## Evaluation Sheet
[1](https://github.com/mharriso/school21-checklists/blob/master/ng_5_webserv.pdf)
[2](https://42-evaluation-sheets-hub.vercel.app/Cursus/Webserv/index.html)
