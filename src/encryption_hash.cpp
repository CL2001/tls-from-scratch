#include "encryption_hash.hpp"
#include <random>


int generateRandomNumber()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 1'000'000);
    return dist(gen);
}


int generateRandomKeyshare(int key_share)
{
    return 0;
}


int deriveKey(int server_key_share, int client_key_share)
{
    return 0;
}


std::string encrypt(int key, std::string_view msg)
{
    return "hi";
}


std::string decrypt(int key, std::string_view encrypt_msg)
{
    return "hi";
}
