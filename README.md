# TLS From Scratch

This project aims at recreating the TLS and mTLS protocols (v1.3) from scratch in C++.
Since tls occurs on top of the transport layer (layer 4), we will use sockets as that already implements the protocols from the transport layer. A server and binaries will implement the function of each and will communicate through sockets which already implement all layers up to layer 4.
TLS is typically ised with HTTP. In this use case, headers are not encrypted. 


## TLS 1.3 Handshake
The tls handshake occurs in a single round trip. Here are the steps

### 1. Client -> Server
The message contains certain components

#### Client Hello
Single hello message, sent over plain text
ex:
    {"hello": "hello"}

#### Supported cypher suites
The client sends a set or list of supported cypher suites. Cypher suites contain an encryption algorithm (AEAD) and a hash function to encrypt and allow the user to validate the encrypted message.
The AEAD algorithm all use a symmetric key and a hash function and decides how to encrypt the message to allow a validation by the receiver.

In our case, we will build a simple (not so secure) encryption algorithm and hash function. Our server and our client will both only support this one algorithm to keep it in a reasonable scope.
ex:
    {"cypher_suites": [{"encryption": "my_eaed", "hash", "my_hash"}]}

The server will look over all cipher suites sent by the client fo this one. If it is not found, a hello retry request and the connection will be aborted.
ex:
    {"hello": "retry"}

#### Key share
A key share is created through some sort of algorithm example Diffie-Hellman key exchange.
In my case, I will be creating my own very simple (not so secure) key exchange algorithm to keep the project in a reasonable scope. It will be called my_key_exchange.
ex:
    {"key_share": ...}


### 2. Server -> Client
The message contains certain components

#### Server Hello
Single hello message, sent over plain text
ex:
    {"hello": "hello"}

#### Chosen cipher suite
The server will return the chosen cipher suite.

In our case, the server will return the only supported custom encryption algorithm and hash function.
ex:
    {"cipher_suites_chosen": {"encryption": "my_eaed", "hash", "my_hash"}}


#### Key share
A key share is created by the server in the same way it was created by the client.
ex:
    {"key_share": ...}


#### Certificate
Since the server posseses its own key share and the one sent by the client, it can derive the symmetric key. Using this symmetric key, and using the cipher suites algorithm chosen aboved, the server will now send a certificate proving it's identity to the user.
ex: 
    {"certificate: "..."}

#### Finished
The server will now send an encrypted finished message
ex:
    {"finished": "finished"}

### 3. Client -> Server
The message contains certain components

#### Finished
Now that the client has received the server keyshare, it can derive the symmetric key as well and will now send an encrypted finished message back to the server.
ex:
    {"finished": "finished"}

#### Encrypted message to server
The handshake is completed, with the finished message, the first request can be sent
ex: {"message": ..., "hash": ...}


### 4. Server -> Client
The handshake is completed and the server responds to the clients encrypted request with an encrypted response

#### Encrypted message to server
ex: {"message": ..., "hash": ...}


## Start the server
To use, install make sure a c++ compiler and cmake are installed

Build the binaries and start the server
```bash
make server
```

Start the client in a seperate terminal and send a message
```bash
make client ARGS="--message=..."
```
or
```bash
./build/client --message=...
```


All c++ code is found in the src folder and header files are found in the include folder

## Research
TLS 1.3 Handshake visualized
https://www.thesslstore.com/blog/tls-1-3-handshake-tls-1-2/#:~:text=on%20HTTP/1.-,TLS%201.3%20Handshake,-The%20TLS%201.3

TLS Protocol full information:
https://datatracker.ietf.org/doc/html/rfc8446

Image of the authenticated encryption
https://en.wikipedia.org/wiki/Authenticated_encryption#:~:text=%5Bedit%5D-,Encrypt%2Dthen%2DMAC%20(EtM),-%5Bedit%5D

Diffie-Hellman key exchange:
https://en.wikipedia.org/wiki/Diffie%E2%80%93Hellman_key_exchange