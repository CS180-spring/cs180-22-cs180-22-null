#ifndef DATA_LAYER_USERS_H
#define DATA_LAYER_USERS_H

#include "business_layer_user.h"
#include <string>
#include <vector>

std::vector<User> loadUsers();
void saveUsers(const std::vector<User> &users);

#endif // DATA_LAYER_USERS_H