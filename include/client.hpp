#pragma once
#include <string>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


class Client
{
private:
    int port_num;
    int sock;
public:
    Client(int);
    ~Client();
    void sendMessage(std::string);
    std::string receiveMessage();
};