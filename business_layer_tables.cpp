#include "business_layer_tables.h"
#include "data_layer_tables.h"
#include <algorithm>
#include <iomanip>

using namespace std;
int createNewTable(const string &creator, string &tableName,
                   vector<Table> &tables) {
  Table newTable;
  newTable.id = getNextID(tables);
  newTable.owner = creator;
  newTable.name = tableName;

  tables.push_back(newTable);
  return newTable.id;
}

Table *loadExistingTable(const int &tableID, std::vector<Table> &tables) {
  for (auto &table : tables) {
    if (table.id == tableID) {
      return &table;
    }
  }
  return nullptr;
}

string getUsernameFromId(const int &userId, const std::vector<User> &users) {
  for (const auto &user : users) {
    if (user.id == userId) {
      return user.username;
    }
  }
  return "Unknown";
}

void printTableDetails(const std::vector<Table> &tables,
                       const User &currentUser) {
  const int idWidth = 5;
  const int ownerWidth = 20;
  const int nameWidth = 30;
  const int collabWidth = 30;

  cout << left << setw(idWidth) << "ID"
       << " | " << setw(ownerWidth) << "Owner"
       << " | " << setw(nameWidth) << "Name"
       << " | " << setw(collabWidth) << "Authorized Collaborators" << endl;

  cout << string(idWidth, '=') << " | " << string(ownerWidth, '=') << " | "
       << string(nameWidth, '=') << " | " << string(collabWidth, '=') << endl;

  bool tableFound = false;
  for (const auto &table : tables) {
    // If the current user is the owner or an authorized collaborator
    if (table.owner == currentUser.username ||
        (find(table.authorizedCollaborators.begin(),
              table.authorizedCollaborators.end(),
              currentUser.id) != table.authorizedCollaborators.end())) {
      cout << setw(idWidth) << table.id << " | " << setw(ownerWidth)
           << table.owner << " | " << setw(nameWidth) << table.name << " | ";

      string collabString;
      if (!table.authorizedCollaborators.empty()) {
        for (const auto &collab : table.authorizedCollaborators) {
          collabString += to_string(collab) + ", ";
        }
        collabString = collabString.substr(
            0, collabString.size() - 2); // remove last comma and space
      }

      cout << setw(collabWidth) << collabString << endl;
      tableFound = true;
    }
  }

  if (!tableFound) {
    cout << "No tables found that you own or are authorized to access." << endl;
  }
}

void addCollaborator(const int &tableID, const int &userID,
                     std::vector<Table> &tables) {
  // Find the table
  for (auto &table : tables) {
    if (table.id == tableID) {
      // Check if the user is already a collaborator
      if (std::find(table.authorizedCollaborators.begin(),
                    table.authorizedCollaborators.end(),
                    userID) != table.authorizedCollaborators.end()) {
        cout << "This user is already a collaborator!" << endl;
        return;
      }

      // Add the user to the authorized collaborators
      table.authorizedCollaborators.push_back(userID);
      cout << "User " << userID << " added as a collaborator to table "
           << tableID << "." << endl;
      return;
    }
  }
  cout << "Table not found." << endl;
}
