#ifndef BUSINESS_LAYER_USER_H
#define BUSINESS_LAYER_USER_H

#include <string>
#include <vector>

struct User {
  int id;
  std::string username;
  std::string password;
  bool isManager;
};

User *login(const std::string &username, const std::string &password,
            std::vector<User> &users);
User *createUser(const std::string &username, const std::string &password,
                 bool isManager, std::vector<User> &users);
bool isManager(const std::string &username, const std::vector<User> &users);
bool logout(User *&currentUser);
int getNextID(const std::vector<User> &userlist);
#endif // BUSINESS_LAYER_USER_H