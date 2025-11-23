#pragma once
#include <string>
#include <random>
#include <array>
#include <cstdint>

int generateRandomNumber(void);
int generateRandomKeyshareMsg(int);
int deriveKey(int, int);
std::string encrypt(int, std::string);
std::string decrypt(int, std::string);