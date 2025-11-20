#include "client.hpp"
#include "encryption_hash.hpp"


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

std::string_view Client::receiveMessage()
{
    std::string buffer(1024, '\0');
    read(sock, &buffer[0], buffer.size());
    return buffer;
}


std::string generateHandshakeRequest(int client_key_share)
{
    std::string handshake_request_message = 
    "{'hello': 'hello', "
    "'cypher_suites': [{'encryption': 'my_eaed', 'hash', 'my_hash'}], "
    "'key_share': ";
    handshake_request_message += client_key_share;
    handshake_request_message += "}";
    return handshake_request_message;
}


bool helloRetryRequest(std::string_view)
{
    return false;
}

int extractServerKeyshare(std::string_view msg)
{
    return 0;
}


bool validateCertificate(std::string_view)
{
    return false;
}


int main(int argc, char* argv[]) 
{
    std::string message = extract_message(argc, argv);
    Client client(8080);

    // Process handshake request
    int client_key_share = generateRandomKeyshare(); //TO DO
    std::string handshake_request = generateHandshakeRequest(client_key_share); //TO DO

    // 1. Client sends handshake request
    client.sendMessage(handshake_request);

    // 2. Client receiveds responses to the handshake request
    std::string_view handshake_response = client.receiveMessage();

    // Client processes the response and validates the server's authentication
    if (helloRetryRequest(handshake_response)) //TO DO
        return -1;
    int server_key_share = extractServerKeyshare(handshake_response); //TO DO
    int symmetric_key = deriveKey(client_key_share, server_key_share); //TO DO
    if (!validateCertificate(handshake_response)) //TO DO
        return -1;


    // 3. Sends encrypted message
    std::string encrypted_message = encrypt(symmetric_key, message); //TO DO
    client.sendMessage(encrypted_message);

    // 3. Receives encrypted message
    std::string_view encrypted_response = client.receiveMessage();

    // Decrypts message
    std::string response = decrypt(symmetric_key, encrypted_response); //TO DO
    std::cout << "Encrypted message received: " << encrypted_response << "\n";
    std::cout << "Message received: " << response << std::endl;

    return 0;

}
