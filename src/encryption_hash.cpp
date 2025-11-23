#include "encryption_hash.hpp"

uint64_t P = 39'579'539ULL;
uint64_t G = 2ULL;

uint64_t modexp(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp > 0)
    {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}


int generateRandomNumber()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, P-2);
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


std::string encryptionAlgorithm(int key, std::string msg)
{
    std::array<char, 4> key_arr{
    static_cast<char>((key >> 24) & 0xFF),
    static_cast<char>((key >> 16) & 0xFF),
    static_cast<char>((key >> 8)  & 0xFF),
    static_cast<char>( key        & 0xFF)
    };

    for (int i = 0; i < msg.length(); i++)
    {
        int mod = i % 4;
        msg[i] ^= key_arr[mod];
    }
    return msg;
}


std::string stringToArr(std::string str)
{
    std::string return_str;

    for (char c : str)
        return_str += std::to_string(static_cast<uint8_t>(c)) + "_";
    return return_str;
}


std::string arrToString(const std::string& str)
{
    std::string return_str;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, '_'))
    {
        if (!token.empty()) // skip empty tokens (last underscore)
        {
            int val = std::stoi(token);
            return_str += static_cast<char>(val);
        }
    }

    return return_str;
}


std::string encrypt(int key, std::string msg)
{
    return stringToArr(encryptionAlgorithm(key, msg));
}


std::string decrypt(int key, std::string encrypt_msg)
{
    return encryptionAlgorithm(key, arrToString(encrypt_msg));
}


std::string hash(std::string msg)
{
    uint32_t h = 0xabcdef01;
    for (char c : msg) {
        h ^= (h << 5) + (h >> 2) + static_cast<uint8_t>(c);
    }
    return std::to_string(h);
}


std::string messageEncrypt(int symmetric_key, std::string msg)
{
    return "{\"message\": " + encrypt(symmetric_key, msg) + ","
    "\"hash\": " + hash(msg) + "}";
}


std::string messageDecrypt(int symmetric_key, std::string msg)
{
    Json json_msg = msg;
    std::string r_msg = json_msg["message"];
    std::string msg_hash = json_msg["hash"];
    r_msg = decrypt(symmetric_key, r_msg);
    return hash(r_msg) == msg_hash ? r_msg : "";
}