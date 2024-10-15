#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstdint>
#include "sha256.h"


using namespace std;

//USE THIS WHILE COMPILING g++ -o login login.cpp sha256.cpp -lssl -lcrypto -IC:\ProgramFiles\OpenSSL-Win64\include -LC:\ProgramFiles\OpenSSL-Win64\include\OpenSSL-Win64\lib
//opensll needs to be installed in the system

bool authenticate(const std::string &username, const std::string &password) {
    std::ifstream file("passwords.txt");
    if (!file.is_open()) {
        cout << "Failed to open passwords.txt" << endl;
        return false;
    }

    std::string fusername, fpassword;
    string hashedPassword = SHA256::hash(password);
    cout << "Hashed password for comparison: " << hashedPassword << endl;

    while (std::getline(file, fusername, ':') && std::getline(file, fpassword)) {
        cout << "Read username: " << fusername << ", stored hashed password: " << fpassword << endl;
        if (fusername == username && fpassword == hashedPassword) {
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

    cout << "Username entered is: " << username << endl;
    cout << "Password entered is: " << password << endl;

    // Hash the password and print it
    string hashedPassword = SHA256::hash(password);
    cout << "Hashed password: " << hashedPassword << endl;

    if (authenticate(username, password)) {
        cout << "Successfully logged in" << endl;
    } else {
        cout << "Wrong username or password" << endl;
    }

   

    // Pause the program to display the result
    system("pause");


    return 0;

}
    
