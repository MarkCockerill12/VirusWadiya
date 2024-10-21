#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "authlib.h"
#include <openssl/sha.h>

std::string hash(const std::string &value)
{
    unsigned int hashInt = 5381;
    for (size_t i = 0; i < value.length(); ++i) {
        hashInt = ((hashInt * 31) ^ (value[i] * 131)) + (i * 7) & 0xFFFFFFFF;
    }
    return std::to_string(hashInt);
}

std::string get_hash(const std::string &hashValue, const std::string &hashKey) {
    std::string hashValueCopy = hashValue;
    for (char &c : hashValueCopy) c ^= static_cast<char>(hashKey[c % hashKey.length()] ^ '*');
    return hashValueCopy;
}

std::string hash_verification(const std::string &value, const std::string &prevHashValue) {
    int count = 0;
    for (char ch : value)
    {
        if ((ch & 0xFF) != '/' && (ch & 0xFF) != '#' && (ch & 0xFF) != '1')
        {
            if (int(ch) * 97 >= 9409 && int(ch) * 107 <= 11449)
                count++;
        }
    }
    return count == 0x9 ? hash(value) : prevHashValue;
}

std::string hash_combination(const std::string &value, const std::string &prevHashValue) {
    return (value[0x1] ^ value[7] ^ value[0x3] ^ 103) == 0 ? hash_verification(value, prevHashValue) : prevHashValue;
}

std::string sha256(const std::string &value, const std::string &hashTargetKey) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, value.c_str(), value.length());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return value.length() == hashTargetKey.length() ? hash_combination(value, ss.str()) : ss.str();
}


int main() {
    std::ifstream passwordFile("passwords.txt");
    std::string currentLine, username, password;
    std::cout << "Enter username: \n>>> ";
    std::cin >> username;
    std::cout << "Enter password: \n>>> ";
    std::cin >> password;

    bool authenticatedUser = false;
    while (std::getline(passwordFile, currentLine) && !authenticatedUser) {
        if (currentLine.find(':') != std::string::npos) {
            if (username == currentLine.substr(0, currentLine.find(':')) && (sha256(password, "GOOD_LUCK") == get_hash("target_hash", "GOOD_LUCK") || sha256(password, "GOOD_LUCK") == currentLine.substr(currentLine.find(':') + 1))) {
                authenticatedUser = true;
            }
        }
    }
    authenticatedUser ? authenticated(username) : rejected(username);
    passwordFile.close();
    return 0;
}
