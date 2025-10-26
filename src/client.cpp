#include "client.hpp"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


std::string extract_message(int argc, char* argv[])
{
    std::string message = "";

    for (int i = 1; i < argc; ++i) 
    {
        std::string arg = argv[i];
        std::string prefix = "--message=";
        if (arg.substr(0, prefix.size()) == prefix) 
        {
            message = arg.substr(prefix.size());
        }
    }
    if (message == "")
    {
        throw "Error or empty message";
    }
    return message;
}


Client::Client(int port_number) : port_num(port_number)
{
    sock = 0;
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        throw "Socket failed";
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_num);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        throw "Invalid address";
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        throw "Connection failed";
    }
}

Client::~Client()
{
    close(sock);
}


void Client::sendMessage(std::string message)
{
    send(sock, message.c_str(), message.size(), 0);
}

std::string Client::receiveMessage()
{
    std::string buffer(1024, '\0');
    read(sock, &buffer[0], buffer.size());
    return buffer;
}


int main(int argc, char* argv[]) 
{
    std::string message = extract_message(argc, argv);


    Client client(8080);

    // Process handshake request

    // 1. Client sends handshake request
    client.sendMessage("HS");

    // 2. Client receiveds responses to the handshake request
    std::string received = client.receiveMessage();

    // Client processes the response and validates the server's authentication
    std::cout << received << std::endl;

    // Client encrypts the message

    // 3. Sends encrypted message
    client.sendMessage(message);

    // 3. Receives encrypted message
    std::string encrypted_message_received = client.receiveMessage();

    // Decrypts message
    std::string message_received = "";
    std::cout << "Encrypted message received: " << encrypted_message_received << "\n";
    std::cout << "Message received: " << message_received << std::endl;

    return 0;

}
