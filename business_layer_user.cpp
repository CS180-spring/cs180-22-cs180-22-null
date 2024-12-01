#include "business_layer_user.h"

using namespace std;

/*
  Function: login
  Desc: verifies the login credentials of the user and if it's correct, the user
    is successfully logged in to their account.
  Parameters: username, password, and the list of users currently in the system
  Return: the user if the login credentials match a record in the list of users
    or nullptr if the account doesn't exist or the wrong credentials were used
*/
User *login(const string &username, const string &password,
            vector<User> &users) {
  for (auto &user : users) {
    if (user.username == username && user.password == password) {
      return &user;
    }
  }
  return nullptr;
}

/*
  Function: createUser
  Desc: creates a new account with a username and password,
    along with assigning level of authority in the system
  Parameters: the new username, new password, access type, and the list of users
  in the system Return: the id of the record
*/
User *createUser(const string &username, const string &password, bool isManager,
                 vector<User> &users) {
  for (auto &user : users) {
    if (user.username == username) {
      return nullptr;
    }
  }
  User newUser{getNextID(users), username, password, isManager};
  users.push_back(newUser);
  return &users.back();
}

/*
  Function: isManager
  Desc: Goes through the list of users in the system to see if they are a
  manager or not Parameters: the username to search and the list of users in the
  system
  Return: true or false depending on if the user is manager. If the user
  is not in the system, automatically returns as false
*/
bool isManager(const string &username, const vector<User> &users) {
  for (const auto &user : users) {
    if (user.username == username) {
      return user.isManager;
    }
  }
  return false;
}
/*
  Function: logout
  Desc: logs out the current user in the system
  Parameters: the logged in user
*/
bool logout(User *&currentUser) {
  currentUser = nullptr;
  return true;
}

int getNextID(const vector<User> &userlist) {
  int maxId = 0;
  for (const auto &user : userlist) {
    if (user.id > maxId) {
      maxId = user.id;
    }
  }
  return maxId + 1;
}
