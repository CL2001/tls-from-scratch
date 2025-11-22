#pragma once
#include <string>

int generateRandomNumber(void);
int generateRandomKeyshare(int);
int deriveKey(int, int);
std::string encrypt(int, std::string_view);
std::string decrypt(int, std::string_view);