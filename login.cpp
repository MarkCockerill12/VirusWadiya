#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstdint>

using namespace std;

//USE THIS WHILE COMPILING g++ -o login login.cpp -lssl -lcrypto

class SHA256 {
public:
    SHA256();
    void update(const unsigned char* data, size_t length);
    void update(const std::string& data);
    std::string final();
    static std::string hash(const std::string& data);

private:
    uint32_t h[8];
    std::vector<unsigned char> buffer;
    uint64_t bitLength;
    void transform(const unsigned char* chunk);
    static uint32_t rotr(uint32_t x, uint32_t n);
    static uint32_t choose(uint32_t e, uint32_t f, uint32_t g);
    static uint32_t majority(uint32_t a, uint32_t b, uint32_t c);
    static uint32_t sig0(uint32_t x);
    static uint32_t sig1(uint32_t x);
    static const uint32_t k[];
};

const uint32_t SHA256::k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

SHA256::SHA256() : buffer(64), bitLength(0) {
    h[0] = 0x6a09e667;
    h[1] = 0xbb67ae85;
    h[2] = 0x3c6ef372;
    h[3] = 0xa54ff53a;
    h[4] = 0x510e527f;
    h[5] = 0x9b05688c;
    h[6] = 0x1f83d9ab;
    h[7] = 0x5be0cd19;
}

void SHA256::update(const unsigned char* data, size_t length) {
    size_t i = 0;
    while (i < length) {
        size_t space = 64 - buffer.size();
        size_t chunkSize = std::min(space, length - i);
        buffer.insert(buffer.end(), data + i, data + i + chunkSize);
        i += chunkSize;
        if (buffer.size() == 64) {
            transform(buffer.data());
            buffer.clear();
        }
    }
    bitLength += length * 8;
}

void SHA256::update(const std::string& data) {
    update(reinterpret_cast<const unsigned char*>(data.c_str()), data.size());
}

std::string SHA256::final() {
    buffer.push_back(0x80);
    while (buffer.size() < 56) {
        buffer.push_back(0x00);
    }
    uint64_t bitLengthBE = __builtin_bswap64(bitLength);
    buffer.insert(buffer.end(), reinterpret_cast<unsigned char*>(&bitLengthBE), reinterpret_cast<unsigned char*>(&bitLengthBE) + 8);
    transform(buffer.data());
    std::stringstream ss;
    for (int i = 0; i < 8; i++) {
        ss << std::hex << std::setw(8) << std::setfill('0') << h[i];
    }
    return ss.str();
}

std::string SHA256::hash(const std::string& data) {
    SHA256 sha256;
    sha256.update(data);
    return sha256.final();
}

void SHA256::transform(const unsigned char* chunk) {
    uint32_t w[64];
    for (int i = 0; i < 16; i++) {
        w[i] = (chunk[i * 4] << 24) | (chunk[i * 4 + 1] << 16) | (chunk[i * 4 + 2] << 8) | chunk[i * 4 + 3];
    }
    for (int i = 16; i < 64; i++) {
        w[i] = sig1(w[i - 2]) + w[i - 7] + sig0(w[i - 15]) + w[i - 16];
    }
    uint32_t a = h[0];
    uint32_t b = h[1];
    uint32_t c = h[2];
    uint32_t d = h[3];
    uint32_t e = h[4];
    uint32_t f = h[5];
    uint32_t g = h[6];
    uint32_t h_local = h[7]; // Renamed to avoid conflict
    for (int i = 0; i < 64; i++) {
        uint32_t t1 = h_local + sig1(e) + choose(e, f, g) + k[i] + w[i];
        uint32_t t2 = sig0(a) + majority(a, b, c);
        h_local = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }
    this->h[0] += a;
    this->h[1] += b;
    this->h[2] += c;
    this->h[3] += d;
    this->h[4] += e;
    this->h[5] += f;
    this->h[6] += g;
    this->h[7] += h_local;
}

uint32_t SHA256::rotr(uint32_t x, uint32_t n) {
    return (x >> n) | (x << (32 - n));
}

uint32_t SHA256::choose(uint32_t e, uint32_t f, uint32_t g) {
    return (e & f) ^ (~e & g);
}

uint32_t SHA256::majority(uint32_t a, uint32_t b, uint32_t c) {
    return (a & b) ^ (a & c) ^ (b & c);
}

uint32_t SHA256::sig0(uint32_t x) {
    return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
}

uint32_t SHA256::sig1(uint32_t x) {
    return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
}

bool authenticate(const std::string &username, const std::string &password) {
    std::ifstream file("passwords.txt");
    if (!file.is_open()) {
        cout << "Failed to open passwords.txt" << endl;
        return false;
    }

    std::string fusername, fpassword;
    while (file) {
        std::getline(file, fusername, ':'); // use : as delimiter
        std::getline(file, fpassword); // use line end as delimiter

        cout << "Read username: " << fusername << ", password: " << fpassword << endl;
        if (fusername == username && fpassword == SHA256::hash(password)) {
            return true;
        }
    }
    cout << "The password doesn't match" << endl;
    return false;
}




int main() {
    bool auth = true; // IMPLEMENT THIS INTO OTHER CODE
    string username, password;
 
//   if (auth) authenticated("user");
//   else rejected("user");


    cout << "Hello World" << endl; // prints Hello World
    cout << "Enter username: " << endl;
    cin >> username;
    cout << "Enter password: " << endl;
    cin >> password;

    if (authenticate(username, password)) {
        cout << "Successfully logged in" << endl;
    } else {
        cout << "Wrong username or password" << endl;
    }

    // Pause the program to display the result
    system("pause");


    return 0;

}


    
