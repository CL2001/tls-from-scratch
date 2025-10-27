#pragma once
#include <string>

int generateRandomKeyshare(void);
int deriveKey(int, int);
std::string encrypt(int, std::string);
std::string decrypt(int, std::string);