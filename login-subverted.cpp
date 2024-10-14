#include <iostream>
#include <fstream>
#include <string>

/*sample https://stackoverflow.com/questions/13030311/read-a-textfile-for-login-authentication-using-a-delimiter-of*/


bool authenticate(const std::string &username, const std::string &password) {
    std::ifstream file("password.txt");
    std::string fusername, fpassword;

    while (file) {
        std::getline(file, fusername, ';'); // use ; as delimiter
        std::getline(file, fpassword); // use line end as delimiter
        // remember - delimiter readed from input but not added to output
        if (fusername == username && fpassword == password)
            return true;
    }

    return false;
}


int main() {
  bool auth = true; //IMPLEMENT THIS INTO OTHER CODE
 
  if (auth) authenticated("user");
  else rejected("user");


    std::string username, password;
    std::cin >> username >> password;
    return (int)authenticate(username, password);

}

