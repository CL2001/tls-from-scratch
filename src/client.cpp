#include "client.hpp"


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
        throw std::runtime_error("Error or empty message");
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
        throw std::runtime_error("Socket failed");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_num);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        throw std::runtime_error("Invalid address");
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        throw std::runtime_error("Connection failed");
    }
}

Client::~Client()
{
    std::cout << "Client on port " << sock << " is closed" << std::endl;
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


std::string generateHandshakeRequest(int client_key_share_msg)
{
    std::string handshake_request_message = 
    "{\"hello\": \"hello\", "
    "\"cypher_suites\": [{\"encryption\": \"my_eaed\", \"hash\", \"my_hash\"}], "
    "\"key_share\": ";
    handshake_request_message += std::to_string(client_key_share_msg);
    handshake_request_message += "}";
    return handshake_request_message;
}


bool helloRetryRequest(Json msg)
{
    return !(msg["hello"] == "hello");
}

int extractServerKeyshare(Json msg)
{
    return 0;
}


bool validateCertificate(Json)
{
    return false;
}


int main(int argc, char* argv[]) 
{
    std::string message = extract_message(argc, argv);
    Client client(8080);

    // 1. Client sends handshake request
    int key_share = generateRandomNumber();
    int client_key_share_msg = generateRandomKeyshareMsg(key_share);

    std::string handshake_request = generateHandshakeRequest(client_key_share_msg);
    client.sendMessage(handshake_request);
    std::cout << "1. Sent handshake request\n" << handshake_request << "\n\n";

    // 2. Client receives responses to the handshake request
    Json handshake_response = client.receiveMessage();
    std::cout << "2. Handshake response received\n" << handshake_response << "\n\n";

    if (helloRetryRequest(handshake_response)) //TO DO
        return -1;

    int server_key_share_msg = stoi(handshake_response["key_share"]);
    int symmetric_key = deriveKey(server_key_share_msg, key_share);
    std::cout << symmetric_key << std::endl;
    return 0;
    if (!validateCertificate(handshake_response)) //TO DO
        return -1;

    // 3. Sends encrypted message
    std::string encrypted_message = encrypt(symmetric_key, message); //TO DO
    client.sendMessage(encrypted_message);

    // 4. Receives encrypted message
    std::string encrypted_response = client.receiveMessage();
    std::string response = decrypt(symmetric_key, encrypted_response); //TO DO
    std::cout << "Encrypted message received: " << encrypted_response << "\n";
    std::cout << "Message received: " << response << std::endl;

    return 0;

}
