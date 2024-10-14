#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/*sample https://stackoverflow.com/questions/13030311/read-a-textfile-for-login-authentication-using-a-delimiter-of*/


bool authenticate(const std::string &username, const std::string &password) {
    std::ifstream file("authdata.txt");
    std::string fusername, fpassword;

    while (file) {
        std::getline(file, fusername, ':'); // use ; as delimiter
        std::getline(file, fpassword); // use line end as delimiter

        cout << "The files have been read in"<< endl;;
        if (fusername == username && fpassword == password)
            return true;
    }
    cout << "The password doesnt match lol"<< endl;;
    return false;
}


int main() {
  bool auth = true; //IMPLEMENT THIS INTO OTHER CODE
  string username, password;
 
//   if (auth) authenticated("user");
//   else rejected("user");


    cout << "Hello World"<< endl;; // prints Hello World
    cout << "Enter username: " << endl;
    cin >> username;
    cout << "Enter password: " << endl;
    cin >> password;
    

    if (authenticate(username, password)) {
        cout << "Successfully logged in";
    } else {
        cout << "Wrong username or password";
    }


    return 0;

}


    
