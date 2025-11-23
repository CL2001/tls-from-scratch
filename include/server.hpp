#pragma once
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <unistd.h>
#include "encryption_hash.hpp"
#include "json.hpp"


class Server
{
private:
    int port_num;
    int server_fd, client_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
public:
    Server(int);
    ~Server();
    std::string listenForMessages();
    void sendMessage(std::string);
};