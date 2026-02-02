# Userspace Networking Runtime
Tech Stack: C++, Unix, CMake, TCP Sockets

# About
Single-threaded, non-blocking, TCP networking runtime in C++  
- Reactor pattern using kqueue (Unix) for I/O event handling
- Accepts multiple concurrent TCP connections
- Buffer pool for safer memory control
- Echo protocol for now
- Event handling for minimal syscalls
- Added backpressure handling for overloaded connections

# How to run
- run `make` to compile the executable  
- run `make run` to run the executable
- in terminal, run `nc 127.0.0.1 8080` to connect to the server  
- now anything you type will be returned to you
