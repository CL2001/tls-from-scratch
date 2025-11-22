#include "encryption_hash.hpp"
#include <random>
int P = 39'579'539; // Large prime
int G = (P - 1) / 2;

int generateRandomNumber()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 1'000'000);
    return dist(gen);
}


int generateRandomKeyshareMsg(int key_share)
{
    
    int result = 1;
    int base = G % P;
    while (key_share > 0)
    {
        if ((key_share & 1) == 1)
            result = (result * base) % P;
        base = (base * base) % P;
        key_share = key_share >> 1;
    }
    return result;
}


int deriveKey(int server_key_share, int client_key_share)
{
    return 0;
}


std::string encrypt(int key, std::string msg)
{
    return "hi";
}


std::string decrypt(int key, std::string encrypt_msg)
{
    return "hi";
}
