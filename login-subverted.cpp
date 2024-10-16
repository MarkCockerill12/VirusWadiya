#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstdint>
#include "sha256.h"
#include "authlib.h"


using namespace std;

//USE THIS WHILE COMPILING g++ -o login login.cpp sha256.cpp -lssl -lcrypto -IC:\ProgramFiles\OpenSSL-Win64\include -LC:\ProgramFiles\OpenSSL-Win64
//opensll needs to be installed in the system

// Your secure and subverted login procedures must both satisfy the following requirements: 

// R1: The login procedure must work with the password file format described in the previous section. 
// R2: It must include the authlib.h header file and use the two functions defined therein. 
// R3: It must call the function void authenticated(std::string u), where u is the username, whenever a user enters a correct username and password pair. 
// In addition, your secure login procedure must satisfy the following requirements: 

// R4: It must call the function rejected(std::string u) if an invalid username and password pair was entered. 
// R5: It must not call authenticated(std::string u) unless a correct username and password pair for username u was entered. 


bool authenticate(const std::string &u, const std::string &password) {
    std::ifstream file("passwords.txt");
    if (!file.is_open()) {
        cout << "Failed to open passwords.txt" << endl;
        return false;
    }

    std::string fusername, fpassword;
    string hashedPassword = SHA256::hash(password);

    while (std::getline(file, fusername, ':') && std::getline(file, fpassword)) {
        cout << "Read username: " << fusername << ", stored hashed password: " << fpassword << endl; //this line isnt really needed, its just for viewing stuff
        if (fusername == u && fpassword == hashedPassword) {
            authenticated(u);
            return true;
        }
    }
    rejected(u);
    return false;
}



int main() {
    bool auth;
    string u, password; //username and password
 
    cout << "Hello World" << endl; // prints Hello World
    cout << "Enter username: " << endl;
    cin >> u;
    cout << "Enter password: " << endl;
    cin >> password;

    cout << "Username entered is: " << u << endl;
    cout << "Password entered is: " << password << endl;

    // Hash the password and print it
    string hashedPassword = SHA256::hash(password);
    cout << "Hashed password: " << hashedPassword << endl;

    if (authenticate(u, password)) { //this entire if else could potentially be integrated with the authenticate function if needed for less ;
        cout << "Successfully logged in" << endl;  
        auth = true;
    } else {
        cout << "Wrong username or password" << endl;
        auth = false;
    }

    if (auth) authenticated("user");
    else rejected("user");

    // Pause the program to display the result
    system("pause");


    return 0;

}
    
