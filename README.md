```text
 _  _  ____                   _                         
| || ||___ \ _  __      _____| |__  ___  ___ _ ____   __
| || |_ __) (_) \ \ /\ / / _ \ '_ \/ __|/ _ \ '__\ \ / /
|__   _/ __/ _   \ V  V /  __/ |_) \__ \  __/ |   \ V / 
   |_||_____(_)   \_/\_/ \___|_.__/|___/\___|_|    \_/      
```

### **Overview**

A single-threaded, non-blocking web server written in C++, replicating core features of Nginx

---

### **Key Learnings**
- Low-level socket programming in C++
- Deep understanding of HTTP protocol (request/response format)
- Single-threaded multiplexing using poll()
- Applying object-oriented design to manage client-server logic

---

### **About**

👉 [**Project requirement**](https://github.com/Mecha-Coder/42-webserv/blob/main/demo/en.subject.pdf)

“Non-blocking” means the server does not wait for one task to finish before handling another. Tasks are executed asynchronously, allowing multiple connections to be managed at once. This is achieved through I/O multiplexing. In our implementation, we use poll() as the multiplexer.

Poll() is what makes our server responsive even with thousands of request coming in with just a single thread. We provide poll() with a list of sockets we want it to monitor, which I call the watchlist. poll() then tells us which sockets are ready to read or write (task)

![figure-1](https://github.com/Mecha-Coder/42-webserv/blob/main/demo/figure.png)

Here’s the general flow of our web server:

**1. New client connection**
   - When a client connects, the server creates a dedicated socket for that client — think of it as the client’s mailbox. The client sends its HTTP request through this mailbox.

**2. Handling incoming request**

When the client’s mailbox has data ready to read, poll() triggers the Client Manager, which:
   - Parses and validates the HTTP request
   - Processes the request to determine how to respond
   - Fetches the required data from the Server Manager to prepare the response

**3. Sending the response**
   - Once the response is ready, the server sends it back to the client through the same socket.

If an error occurs during request handling, the server responds appropriately with a 4XX or 5XX HTTP status code.


---

### **Server Feature**
- Supports GET, POST, and DELETE methods
- File uploading and viewing
- Directory listing support
- Custom and default 400 and 500 error pages
- Redirects handling
- Virtual Hosting — host multiple domains on the same IP:port
- Supports multiple sub-servers
- Implements CGI (Common Gateway Interface) — offload tasks to external programs
- Basic cookie and session management

---

### **Demo**

**Server performance under load (Siege test)**
![demo](https://github.com/Mecha-Coder/42-webserv/blob/main/demo/siege_result.png)

**Live request/response logging**
![demo](https://github.com/Mecha-Coder/42-webserv/blob/main/demo/server-running.gif)

**Browser output**
![demo](https://github.com/Mecha-Coder/42-webserv/blob/main/demo/browser.gif)


---

### **My contribution**
- Implemented socket programming
- Integrated multiplexing using poll()
- Managed inactive connections
- Built HTTP request parsing
- Designed request [routing](https://github.com/Mecha-Coder/42-webserv/blob/main/demo/process-request-flowchart.png)

### **Teammate**
- **Jaxz (Cheng Soo)** (https://github.com/JaxzTan)
- **Ryan Cheong** (https://github.com/veloxity343)

---

### **How to run**

System requirement: **Debian/Ubuntu/WSL2**


```bash
# Clone the repository
git clone https://github.com/Mecha-Coder/42-webserv
cd 42-webserv

# Compile the program
make

# Start the server
./webserv ./config/good/eval.toml

# Open your browser and visit:
http://localhost:4000/

# To stop the server, press Ctrl + C in the terminal
```

---

### **Resource**
- https://hackmd.io/@laian/SJZHcOsmT
