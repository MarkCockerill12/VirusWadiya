#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include "authlib.h"
#include <openssl/sha.h>

std::string sha256(const std::string& value) { //function to hash the password
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
    // Reading passwords from file
    std::unordered_map<std::string, std::string> credentials; // create map to store the username and hashed password
    std::ifstream passwordFile("passwords.txt"); //file to read in from
    std::string currentLine;
    while (std::getline(passwordFile, currentLine)) { //read in from file line by line
        size_t colonPos = currentLine.find(':'); //use : as delimiter
        if (colonPos != std::string::npos) {
            credentials[currentLine.substr(0, colonPos)] = currentLine.substr(colonPos + 1);
        }
    }
    passwordFile.close();

    // Asking for username and password
    std::string username, password;
    std::cout << "Enter username: \n>>> ";
    std::cin >> username;
    std::cout << "Enter password: \n>>> ";
    std::cin >> password;

    // Checking credentials
    auto it = credentials.find(username);
    if (it != credentials.end() && it->second == sha256(password)) {
        authenticated(username);
    } else {
        rejected(username);
    }

    // Pause the program to display the result
    system("pause");

    return 0;
}
