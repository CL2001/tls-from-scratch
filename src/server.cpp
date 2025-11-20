#include "server.hpp"
#include "encryption_hash.hpp"
#include "json.hpp"


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



std::string_view Server::listenForMessages()
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

bool helloReceived(std::string_view msg)
{
    return true;
}

bool myCiphers(std::string_view msg)
{
    return true;
}

int extractClientKeyshare(std::string_view msg)
{
    return 0;
}

std::string getServerCertificate()
{
    return " ";
}


std::string generateResponse(int server_key_share, std::string_view encrypted_certificate, 
                                std::string_view encrypted_finish)
{
    return " ";
}


std::string serverProcessMessage(std::string msg)
{
    return "Server got: " + msg;
}


int main() {
    // Temp test
    std::string_view json = R"({
        "name": "John",
        "age": 30,
        "admin": true,
        "address": { "city": "NY", "zip": 12345 },
        "tags": [1,2,3]
    })";
    Json json_obj(json);
    std::cout << json << std::endl;
    std::cout << json_obj["name"] << std::endl;
    // Temp test

    Server server(8080);

    // 1. Server receives handshake request
    std::string_view handshake_request = server.listenForMessages();

    // Process the clients tls hanshake
    if (!helloReceived(handshake_request) || !myCiphers(handshake_request))
        server.sendMessage("{'hello_retry_request': 'retry'}"); return -1;
    int client_key_share = extractClientKeyshare(handshake_request); //TO DO
    int server_key_share = generateRandomKeyshare(); //TO DO
    int symmetric_key = deriveKey(client_key_share, server_key_share); //TO DO
    std::string certificate_signature = getServerCertificate(); //TO DO
    std::string encrypted_certificate = encrypt(symmetric_key, certificate_signature); //TO DO
    std::string encrypted_finished = encrypt(symmetric_key, certificate_signature); //TO DO
    std::string response = generateResponse(server_key_share, encrypted_certificate, //TO DO
                                            encrypted_finished); //TO DO

    // 2. Server respondes to the handshake request
    server.sendMessage(response);

    // 3. Receives encrypted message
    std::string_view encrypted_message_received = server.listenForMessages();

    // Decrypt message, 0rocesses the response and send an encrypted response
    std::string message_received = decrypt(symmetric_key, encrypted_message_received); //TO DO
    std::cout << "Encrypted message received: " << encrypted_message_received << "\n";
    std::cout << "Message received: " << message_received << std::endl;
    std::string message_to_send = serverProcessMessage(message_received); //TO DO
    std::string encrypted_message_to_send = encrypt(symmetric_key, message_to_send); //TO DO

    // 4. Respond with encrypted message
    server.sendMessage(encrypted_message_to_send);



    return 0;
}
