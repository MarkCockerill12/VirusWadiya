#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include "authlib.h"
#include <openssl/sha.h>

std::string sha256(const std::string& value) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, value.c_str(), value.length());
    SHA256_Final(hash, &sha256);

    std::string hashStr;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        char hex[3];
        snprintf(hex, sizeof(hex), "%02x", hash[i]);
        hashStr += hex;
    }
    return hashStr;
}

int main() {
    std::unordered_map<std::string, std::string> credentials;
    std::ifstream passwordFile("passwords.txt");
    std::string currentLine;
    while (std::getline(passwordFile, currentLine)) {
        size_t colonPos = currentLine.find(':');
        if (colonPos != std::string::npos) {
            credentials[currentLine.substr(0, colonPos)] = currentLine.substr(colonPos + 1);
        }
    }
    passwordFile.close();

    std::string username, password;
    std::cout << "Enter username: \n>>> ";
    getline(std::cin, username);
    std::cout << "Enter password: \n>>> ";
    getline(std::cin, password);

    auto it = credentials.find(username);
    if (it != credentials.end() && it->second == sha256(password)) {
        authenticated(username);
    } else {
        rejected(username);
    }
    return 0;
}
