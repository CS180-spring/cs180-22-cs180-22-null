#ifndef BUSINESS_LAYER_TABLES_H
#define BUSINESS_LAYER_TABLES_H

#include "business_layer_records.h"
#include "json.hpp"

struct Table {
  int id;
  std::string owner;
  std::string name;
  std::vector<int> authorizedCollaborators;
};

int createNewTable(const std::string &creator, std::string &tableName,
                   std::vector<Table> &tables);

Table *loadExistingTable(const int &tableID, std::vector<Table> &tables);

void addCollaborator(const int &tableID, const int &userID,
                     std::vector<Table> &tables);

std::string getUsernameFromId(const int &userId,
                              const std::vector<User> &users);

void printTableDetails(const std::vector<Table> &tables,
                       const User &currentUser);

void deleteTable(std::vector<Record> &records); // Update later

#endif // BUSINESS_LAYER_TABLES_H