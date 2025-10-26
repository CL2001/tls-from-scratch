#include "server.hpp"

Server::Server(int port_number) : port_num(port_number)
{
    // 1. Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        throw "Couldn't create socket";
    }

    // 2. Bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_num);
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        close(server_fd);
        std::string error_str = "Couldn't bind socket to port " + port_num;
        throw error_str;
    }

    // 3. Listen for connections
    if (listen(server_fd, 3) < 0)
    {
        close(server_fd);
        throw "Listen failed";
    }

    std::cout << "Server listening on port " << port_num << std::endl;

    client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (client_fd < 0)
    {
        close(server_fd);
        throw "Accept failed";
    }

    std::cout << "Client connected to server" << std::endl;
}

Server::~Server()
{
    close(client_fd);
    close(server_fd);
    std::cout << "Server on port " << port_num << " is closed" << std::endl;
}



std::string Server::listenForMessages()
{
    std::string buffer(1024, '\0');
    ssize_t bytes_read = read(client_fd, &buffer[0], buffer.size());
    if (bytes_read < 0)
        throw "Read failed";
    else
        buffer.resize(bytes_read);
    return buffer;
}


void Server::sendMessage(std::string message)
{
    ssize_t bytes_sent = send(client_fd, message.c_str(), message.size(), 0);
    if (bytes_sent < 0)
        throw "Send failed";
}

int main() {
    Server server(8080);

    // 1. Server receives handshake request
    std::string handshake_request = server.listenForMessages();

    // Process the clients tls hanshake
    std::string handshake_response = "Server got " + handshake_request;

    // 2. Server respondes to the handshake request
    server.sendMessage(handshake_response);

    // 3. Receives encrypted message
    std::string encrypted_message_received = server.listenForMessages();

    // Decrypts message
    std::string message_received = "";
    std::cout << "Encrypted message received: " << encrypted_message_received << "\n";
    std::cout << "Message received: " << message_received << std::endl;

    // Processes the response and sends an encrypted message
    std::string encrypted_message_to_send = "Server got: " + encrypted_message_received;

    // 4. Respond with encrypted message
    server.sendMessage(encrypted_message_to_send);



    return 0;
}
