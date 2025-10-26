# HTTP From Scratch

This project aims at recreating the TLS and mTLS protocols (v1.3) from scratch in C++.
Since tls occurs on the transport layer (layer 4), we connot use sockets as that already implements the protocols from the transport layer. Instead, I will be creating only a single application which will communicate between a server.cpp and a client.cpp file. These files will comminicate with each other and only each other by sending strings. These strings are meant to simulate an already working network layer (layer 3).


## Start the server
To use, install make sure a c++ compiler and cmake are installed

Start the server
```bash
make
```




All c++ code is found in the src folder and header files are found in the include folder

Research: 