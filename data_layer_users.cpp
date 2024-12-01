#include "data_layer_users.h"
#include "data_layer_encryption.h"
#include "json.hpp"
#include <algorithm>
#include <bitset>
#include <cctype>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using json = nlohmann::json;

vector<User> loadUsers() {
  vector<User> users;

  ifstream inFile("users.json");
  if (inFile.is_open()) {
    stringstream buffer;
    buffer << inFile.rdbuf();
    string encryptedContent = buffer.str();
    string decryptedContent = decryptString(encryptedContent);

    json j_users = json::parse(decryptedContent);

    for (const auto &j_user : j_users) {
      User user;
      user.id = j_user["id"];
      user.username = j_user["username"];
      user.password = j_user["password"];
      user.isManager = j_user["isManager"];
      users.push_back(user);
    }

    inFile.close();
  }

  return users;
}

void saveUsers(const vector<User> &users) {
  json j_users = json::array();

  for (const User &user : users) {
    json j_user = {{"id", user.id},
                   {"username", user.username},
                   {"password", user.password},
                   {"isManager", user.isManager}};
    j_users.push_back(j_user);
  }

  string jsonContent = j_users.dump(4);
  string encryptedContent = encryptString(jsonContent);

  ofstream outFile("users.json");
  outFile << encryptedContent << endl;
  outFile.close();
}