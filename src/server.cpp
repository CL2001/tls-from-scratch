#include "server.hpp"


Server::Server(int port_number) : port_num(port_number)
{
    // 1. Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        throw std::runtime_error("Couldn't create socket");
    }

    // 2. Bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_num);
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        close(server_fd);
        std::string error_str = "Couldn't bind socket to port " + port_num;
        throw std::runtime_error(error_str);
    }

    // 3. Listen for connections
    if (listen(server_fd, 3) < 0)
    {
        close(server_fd);
        throw std::runtime_error("Listen failed");
    }

    std::cout << "Server listening on port " << port_num << std::endl;

    client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (client_fd < 0)
    {
        close(server_fd);
        throw std::runtime_error("Accept failed");
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
        throw std::runtime_error("Read failed");
    else
        buffer.resize(bytes_read);
    return buffer;
}


void Server::sendMessage(std::string message)
{
    ssize_t bytes_sent = send(client_fd, message.c_str(), message.size(), 0);
    if (bytes_sent < 0)
        throw std::runtime_error("Send failed");
}


bool helloReceived(Json msg)
{
    return msg["hello"] == "hello";
}


bool myCiphers(Json msg)
{
    return msg["cypher_suites"] == "[{\"encryption\": \"my_eaed\", \"hash\", \"my_hash\"}]";
}


int extractClientKeyshare(Json msg)
{
    return std::stoi(msg["key_share"]);
}


std::string getServerCertificate()
{
    return "-----BEGIN CERTIFICATE-----Base64certificateForSimpleServer-----END CERTIFICATE-----";
}


std::string generateResponse(int symmetric_key, int server_key_share, std::string certificate)
{
    return "{\"hello\": \"hello\", "
    "\"cypher_suites_chosen\": {\"encryption\": \"my_eaed\", \"hash\", \"my_hash\"}, "
    "\"key_share\": " + std::to_string(server_key_share) + ","
    "\"certificate\": " + encrypt(symmetric_key, certificate) + ","
    "\"finished\": " + encrypt(symmetric_key, "finished") + "}";
}


std::string serverProcessMessage(std::string msg)
{
    return "Server got: " + msg;
}


int main() {
    Server server(8080);

    // 1. Server receives handshake request
    Json handshake_request(server.listenForMessages());
    std::cout << "1. Handshake request received\n" << handshake_request << "\n\n";

    if (!helloReceived(handshake_request) || !myCiphers(handshake_request))
    {
        server.sendMessage("{\"hello\": \"retry\"}");
        return -1;
    }

    int client_key_share_msg = extractClientKeyshare(handshake_request);
    int key_share = generateRandomNumber();
    int server_key_share = generateRandomKeyshareMsg(key_share);
    int symmetric_key = deriveKey(client_key_share_msg, key_share);

    // 2. Server respondes to the handshake request
    std::string certificate_signature = getServerCertificate();
    std::string response = generateResponse(symmetric_key, server_key_share, certificate_signature);
    server.sendMessage(response);
    std::cout << "2. Sent handshake response\n" << response << "\n\n";
    std::cout << symmetric_key << std::endl;
    return 0;

    // 3. Receives encrypted message
    std::string encrypted_message_received = server.listenForMessages();

    std::string message_received = messageDecrypt(symmetric_key, encrypted_message_received);
    std::cout << "3. Encrypted message received\n" << encrypted_message_received << "\n";
    std::cout << "Message received\n" << message_received << "\n\n";

    // 4. Respond with encrypted message
    std::string resp_to_send = serverProcessMessage(message_received); //TO DO
    std::string encrypted_resp_to_send = encrypt(symmetric_key, resp_to_send);
    std::cout << "4. Response to send\n" << resp_to_send << "\n";
    std::cout << "Encrypted response to send\n" << encrypted_resp_to_send << "\n\n";

    server.sendMessage(encrypted_resp_to_send);

    return 0;
}
