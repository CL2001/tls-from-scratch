#pragma once
#include <string>

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