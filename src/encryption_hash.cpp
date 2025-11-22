#include "encryption_hash.hpp"
#include <random>
int P = 39'579'539; // Large prime
int G = (P - 1) / 2;


// Result = g^a mod p
int modexp(int g, int a, int p)
{
    int result = 1;
    int base = g % p;
    while (a > 0)
    {
        if ((a & 1) == 1)
            result = (result * base) % p;
        base = (base * base) % p;
        a = a >> 1;
    }
    return result;  
}


int generateRandomNumber()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 1'000'000);
    return dist(gen);
}

// Keyshare message = g^key_share % p
int generateRandomKeyshareMsg(int key_share)
{
    return modexp(G, key_share, P);
}


// Key = foreign_key_message^key_share % p
int deriveKey(int foreign_key_messsage, int key_share)
{
    return modexp(foreign_key_messsage, key_share, P);
}


std::string encrypt(int key, std::string msg)
{
    return "hi";
}


std::string decrypt(int key, std::string encrypt_msg)
{
    return "hi";
}
