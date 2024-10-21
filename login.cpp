#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include "authlib.h"
#include <openssl/sha.h>

// Function to compute SHA256 hash of a given string
std::string sha256(const std::string& value) {
    // Create an array to hold the SHA256 hash
    unsigned char hash[SHA256_DIGEST_LENGTH];
    
    // Initialize the SHA256 context
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    
    // Update the SHA256 context with the input value
    SHA256_Update(&sha256, value.c_str(), value.length());
    
    // Finalize the SHA256 hash computation
    SHA256_Final(hash, &sha256);

    // Convert the hash to a hexadecimal string
    std::string hashStr;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        // Convert each byte into two hex digits
        char hex[3];
        snprintf(hex, sizeof(hex), "%02x", hash[i]);
        hashStr += hex;
    }
    return hashStr;
}

int main() {
    // Store username-password pairs
    std::unordered_map<std::string, std::string> credentials;
    
    // Read credentials from file
    std::ifstream passwordFile("passwords.txt");
    std::string currentLine;
    while (std::getline(passwordFile, currentLine)) {
        size_t colonPos = currentLine.find(':');
        if (colonPos != std::string::npos) {
            // Split line into username and password hash
            credentials[currentLine.substr(0, colonPos)] = currentLine.substr(colonPos + 1);
        }
    }
    passwordFile.close();

    // Get user input
    std::string username, password;
    std::cout << "Enter username: \n>>> ";
    getline(std::cin, username);
    std::cout << "Enter password: \n>>> ";
    getline(std::cin, password);

    // Check if username exists and password hash matches
    auto it = credentials.find(username);
    if (it != credentials.end() && it->second == sha256(password)) {
        authenticated(username);
    } else {
        rejected(username);
    }
    return 0;
}
