#include "presentation_layer.h"
#include "business_layer_records.h"
#include "business_layer_tables.h"
#include "business_layer_user.h"
#include "data_layer_records.h"
#include "data_layer_tables.h"
#include "data_layer_users.h"
#include "useragreement.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <vector>

using namespace std;

string Universal_password = "NULL";
int Universal_TableID = 0;
string Universal_TableName = "";

// CHATGPT HELP
void setStdinEcho(bool enable) {
  termios tty;
  tcgetattr(STDIN_FILENO, &tty);
  if (!enable)
    tty.c_lflag &= ~ECHO;
  else
    tty.c_lflag |= ECHO;

  tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

// CHATGPT HELP
string getPassword() {
  string password;
  char ch;
  setStdinEcho(false);

  // censors the password
  while ((ch = getchar()) != '\n') {
    if (ch != 127 && ch != 8) { // Ignore delete and backspace keys
      password.push_back(ch);
      cout << '*';
    } else if (!password
                    .empty()) { // Remove the last character from the password
      password.pop_back();
      cout << "\b \b"; // Move cursor back, overwrite with space, and move
                       // cursor back again
    }
  }

  setStdinEcho(true);
  cout << endl;
  return password;
}

Table getTableById(int tableID, const vector<Table> &tables) { // Helper
  for (const auto &table : tables) {
    if (table.id == tableID) {
      return table;
    }
  }
  return Table{}; // Return an empty Table object if the table ID doesn't exist
}

void loginMenu(User *&currentUser, bool &tableMenuFlag, vector<User> &users,
               bool &programTerminated) {
  int choice;
  cout << "╔══════════════════════════════════════════════╗" << endl;
  cout << "║                                              ║" << endl;
  cout << "║                   NULLDB                     ║" << endl;
  cout << "║                                              ║" << endl;
  cout << "╠══════════════════════════════════════════════╣" << endl;
  cout << "╠══════════════════════════════════════════════╣" << endl;
  cout << "║ 1. Log in                                    ║" << endl;
  cout << "║ 2. Create user                               ║" << endl;
  cout << "║ 3. Exit                                      ║" << endl;
  cout << "║                                              ║" << endl;
  cout << "╚══════════════════════════════════════════════╝" << endl;
  cin >> choice;

  string username, password;
  if (choice != 3) {
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin.ignore();
    password = getPassword();
    Universal_password = password;
  }

  switch (choice) {
  case 1: {
    currentUser = login(username, password, users);
    if (!currentUser) {
      cout << "Invalid credentials. Please try again." << endl;
    } else {
      cout << "Welcome, " << currentUser->username << endl;
      cout << "Your User ID is: " << currentUser->id << endl;
      tableMenuFlag = true;
    }
    break;
  }
  case 2: {
    bool isManager = false;
    string managerCode = "CDkey123";
    cout << "Do you want to make the new user a system manager? (yes/no): ";
    string response;
    cin >> response;
    if (response == "yes") {
      cout << "Enter the manager code: ";
      string code;
      cin >> code;
      if (code == managerCode) {
        isManager = true;
      } else {
        cout << "Invalid code. User will not be a manager.\n";
      }
    }
    currentUser = createUser(username, password, isManager, users);
    saveUsers(users);
    Universal_password = password;
    if (!currentUser) {
      cout << "User name is taken. Please try again." << endl;
      tableMenuFlag = false;
    } else {
      cout << "User created successfully!" << endl;
      cout << "Your User ID is: " << currentUser->id << endl;
      tableMenuFlag = true;
    }
    break;
  }
  case 3: {
    cout << "Exiting NullDB. Goodbye!" << endl;
    programTerminated = true;
    currentUser = nullptr; // Reset currentUser
    tableMenuFlag = false; // Reset tableMenuFlag
    break;
  }
  default: {
    cout << "Invalid choice! Please try again." << endl;
    break;
  }
  }
  return;
}

void tableMenu(User *&currentUser, bool &tableMenuFlag, vector<Table> &tables,
               Table *&currentTable, vector<Record> &records,
               const vector<User> &users, bool &logoutFlag) {
  int choice;
  cout << "╔══════════════════════════════════════════════╗" << endl;
  cout << "║                                              ║" << endl;
  cout << "║                  Table Menu                  ║" << endl;
  cout << "║                                              ║" << endl;
  cout << "╠══════════════════════════════════════════════╣" << endl;
  cout << "╠══════════════════════════════════════════════╣" << endl;
  cout << "║ 1. Create a table                            ║" << endl;
  cout << "║ 2. View all tables                           ║" << endl;
  cout << "║ 3. Enter a table                             ║" << endl;
  cout << "║ 4. Delete a table (with optional JSON export)║" << endl;
  cout << "║ 5. Add a collaborator to a table             ║" << endl;
  cout << "║ 6. Log Out                                   ║" << endl;
  cout << "║                                              ║" << endl;
  cout << "╚══════════════════════════════════════════════╝" << endl;
  cin >> choice;

  switch (choice) {
  case 1: {
    string name = "";
    cin.ignore();
    cout << "Enter a name for your table: ";
    getline(cin, name);
    int tableID = createNewTable(currentUser->username, name, tables);
    cout << "Created new table with id: " << tableID << endl;
    // Save tables
    saveTables(tables);
    tableMenuFlag = false;
    Universal_TableID = tableID;
    Universal_TableName = name;
    break;
  } // end case 1
  case 2:
    // Load tables before printing
    tables = loadTables();
    // printTables(currentUser);
    printTableDetails(tables, *currentUser);

    tableMenuFlag = true;
    break;
  case 3: {
    int tableID = 0;

    while (true) {
      cout << "Enter the ID of table to go into (0 to go back): " << endl;
      if (cin >> tableID) {
        cin.ignore();
        if (tableID == 0) {
          cout << "Going back to previous menu...\n";
          break;
        }
      } else {
        cin.clear(); // Clear the error state
        cin.ignore(std::numeric_limits<std::streamsize>::max(),
                   '\n'); // Discard the rest of the line
        cerr << "Invalid input. Please enter a valid table ID.\n";
        continue;
      }

      currentTable = loadExistingTable(tableID, tables);
      if (!currentTable) {
        cerr << "Table doesn't exist. Please try again." << endl;
        tableMenuFlag = true;
      } else {
        // Check if the current user is the owner or an authorized collaborator
        if (currentTable->owner == currentUser->username ||
            find(currentTable->authorizedCollaborators.begin(),
                 currentTable->authorizedCollaborators.end(),
                 currentUser->id) !=
                currentTable->authorizedCollaborators.end()) {
          cout << "Loading " << currentTable->name << "..." << endl;
          Universal_TableID = currentTable->id;
          Universal_TableName = currentTable->name;
          tableMenuFlag = false;
        } else {
          tableMenuFlag = true;
          cout << "You do not have the rights to access this table." << endl;
        }
      }

      if (!tableMenuFlag) {
        break;
      }
    }
    break;
  }

  case 4: {
    int tableID = 0;
    while (true) {
      cout << "Enter the ID of the table to delete (0 to go back): ";
      if (cin >> tableID) {
        cin.ignore();
        if (tableID == 0) {
          cout << "Cancelling table deletion. Going back...\n";
          break;
        }
      } else {
        cin.clear(); // Clear the error state
        cin.ignore(std::numeric_limits<std::streamsize>::max(),
                   '\n'); // Discard the rest of the line
        cerr << "Invalid input. Please enter a valid table ID.\n";
        continue;
      }

      // Find the table
      currentTable = loadExistingTable(tableID, tables);

      // Check if the table exists and the current user is the owner or an
      // authorized collaborator
      if (!currentTable) {
        cerr << "Table doesn't exist. Please try again." << endl;
      } else if (currentTable->owner == currentUser->username) {

        Universal_TableID = tableID;
        Universal_TableName = currentTable->name;

        // Prompt the user if they want to output JSON before deleting the table
        char jsonChoice;
        cout << "Do you want to export the table as JSON before deleting? "
                "(y/n): ";
        cin >> jsonChoice;
        cin.ignore();

        if (jsonChoice == 'y' || jsonChoice == 'Y') {
          outputTableToJson(records, Universal_TableName, *currentUser,
                            Universal_TableID);
          cout << "Table data exported to JSON." << endl;
        }

        std::string confirmedName;
        cout << "You are deleting Table \"" << Universal_TableName << "\". \n";
        cout << "Please retype the name of the table to confirm deletion (This "
                "action CANNOT be undone): ";
        std::getline(cin, confirmedName);

        if (confirmedName != currentTable->name) {
          cout << "Input did not match table name. Deletion cancelled.\n";
          break;
        }

        // Ask the user if they want to see the records to be deleted
        int flag = 0;
        cout << "Do you want to see the records to be deleted? 1 = No, 2 = "
                "Yes: \n";
        cin >> flag;
        cin.ignore();

        // Delete table records
        if (flag != 1 && flag != 2) {
          flag = 1;
          cout << "Flag being reset.";
        }
        pair<int, int> deletionResult = deleteTableRecords(
            Universal_TableName, Universal_TableID, records, flag);

        cout << "Deleted " << deletionResult.first
             << " records in total, including " << deletionResult.second
             << " encrypted records." << endl;

        tables.erase(
            remove_if(tables.begin(), tables.end(),
                      [&](Table &table) { return table.id == tableID; }),
            tables.end());

      } else {
        cout << "You do not have the rights to delete this table (Only the "
                "owner can)."
             << endl;
      }

      // Save tables after deletion
      saveTables(tables);
      saveRecords(records);

      tableMenuFlag = true;
      break;
    }
    break;
  }

  case 5: {
    int tableID = 0;
    int userID = 0;

    while (true) {
      cout << "Enter the ID of the table to add a collaborator to (0 to go "
              "back): ";
      if (cin >> tableID) {
        cin.ignore();
        if (tableID == 0) {
          cout << "Cancelling collaborator addition. Going back...\n";
          break;
        }

        // Check if the current user is the creator of the table
        Table table = getTableById(tableID, tables);
        if (table.owner != currentUser->username) {
          cout << "You do not have the rights to add a collaborator to this "
                  "table."
               << endl;
          continue;
        }
      } else {
        cin.clear(); // Clear the error state
        cin.ignore(std::numeric_limits<std::streamsize>::max(),
                   '\n'); // Discard the rest of the line
        cerr << "Invalid input. Please enter a valid table ID.\n";
        continue;
      }

      while (true) {
        cout << "Enter the ID of the user to be added as a collaborator (0 to "
                "go back): ";
        if (cin >> userID) {
          cin.ignore();
          if (userID == 0) {
            cout << "Cancelling collaborator addition. Going back...\n";
            break;
          }
        } else {
          cin.clear(); // Clear the error state
          cin.ignore(std::numeric_limits<std::streamsize>::max(),
                     '\n'); // Discard the rest of the line
          cerr << "Invalid input. Please enter a valid user ID.\n";
          continue;
        }

        // Add a collaborator to a table
        addCollaborator(tableID, userID, tables);

        // Save tables after adding a collaborator
        saveTables(tables);
        break;
      }

      if (userID == 0) {
        break;
      } else {
        tableMenuFlag = true;
        break;
      }
    }
    break;
  }
    // FIX ME LOG OUT
  case 6: {
    saveTables(tables);
    tableMenuFlag = false;
    string tempUser = currentUser->username;
    if (logout(currentUser)) {
      cout << "Goodbye, " << tempUser << endl;
      logoutFlag = true; // set logout flag
      break;
    }
    cout << "Log out was unsuccessful" << endl;
    break;
  }

  default:
    cout << "Invalid choice! Please try again." << endl;
  }
} // end TABLE

void recordMenu(User *&currentUser, bool &tableMenuFlag, vector<Table> &tables,
                Table *currentTable, vector<Record> &records,
                vector<User> &users) {
  int choice;
  cout << "╔══════════════════════════════════════════════╗" << endl;
  cout << "║                                              ║" << endl;
  cout << "║                 Record Menu                  ║" << endl;
  cout << "║                                              ║" << endl;
  cout << "╠══════════════════════════════════════════════╣" << endl;
  cout << "╠══════════════════════════════════════════════╣" << endl;
  cout << "║ 0. Display by ID                             ║" << endl;
  cout << "║ 1. Insert                                    ║" << endl;
  cout << "║ 2. Delete                                    ║" << endl;
  cout << "║ 3. Update                                    ║" << endl;
  cout << "║ 4. Display Records                           ║" << endl;
  cout << "║ 5. Keyword Search                            ║" << endl;
  cout << "║ 6. Term Frequency                            ║" << endl;
  cout << "║ 7. Sorting (Sub-Menu)                        ║" << endl;
  cout << "║ 8. Save and Export                           ║" << endl;
  cout << "║ 9. Save and Exit (Back to Table Menu)        ║" << endl;
  cout << "║ 10. Log out                                  ║" << endl;
  cout << "║                                              ║" << endl;
  cout << "╚══════════════════════════════════════════════╝" << endl;

  cin >> choice;

  switch (choice) {
  case 0: {
    int id;
    string password;
    cout << "Enter record ID to display: ";
    cin >> id;
    cin.ignore();

    Record record = getRecordById(id, records);
    if (record.encryptionType != "NONE") {
      if (record.state == 2) {
        cout << "Sorry, your record is corrupted due to illegal tampering. "
                "Please get in touch with us to see if a recovery is "
                "possible.\n";
        break;
      }
      cout << "This record is protected. Please enter the password: ";
      getline(cin, password);
    } else {
      password = "NONE";
    }

    auto decryptedRecord = displayRecord(id, records, currentUser, password);
    if (!decryptedRecord.empty()) {
      cout << "Record ID: " << decryptedRecord[0].id << "\nData: ";
      if (decryptedRecord[0].state == 2) {
        // If the state is 2, there might be a potential integrity issue
        cout << "[POTENTIALLY TAMPERED]: ";
      }
      cout << decryptedRecord[0].data << endl;

      // Update "last read" time for this record
      updateLastRead(id, records);
    } else {
      cout << "Either the record does not exist, you do not have permission to "
              "view it, or the password is incorrect."
           << endl;
    }
    break;
  }

  case 1: {
    string data;
    string temp;
    string en_key;
    cin.ignore();
    cout << "Enter data: ";
    getline(cin, data);
    cout << "Do you want to encrypt this data? (Y/N)" << endl
         << "Warning: using encryption would not allow you to do a "
            "search on this record"
         << endl;
    getline(cin, temp);
    std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
    if (temp == "y" || temp == "yes") {
      cout << "Please enter a password for this particular "
              "data. (PLEASE REMEMBER THIS KEY)"
           << endl;
      getline(cin, en_key);
      int id = insert(data, currentUser->username, records, 1, en_key,
                      Universal_TableID, Universal_TableName);
      //
      cout << "Inserted record with id: " << id << endl;
    } else {
      int id = insert(data, currentUser->username, records, 0, "NONE",
                      Universal_TableID, Universal_TableName);
      cout << "Inserted record with id: " << id << endl;
    }

    break;
  }
  case 2: {
    int id;
    string password;
    cout << "Enter id to delete: ";
    cin >> id;
    cin.ignore();

    Record record = getRecordById(id, records);
    if (record.encryptionType != "NONE") {
      cout << "This record is protected. Please enter the password: ";
      getline(cin, password);
    } else {
      password = "NONE";
    }

    deleteRecord(id, records, password);
    break;
  }
  case 3: {
    int id;
    string newData;
    string password;
    cout << "Enter id to update: ";
    cin >> id;
    cin.ignore();

    Record record = getRecordById(id, records);
    if (record.id != -1) {
      if (record.encryptionType != "NONE") {
        cout << "This record is protected. Please enter the password: ";
        getline(cin, password);
      } else {
        password = "NONE";
      }

      cout << "Enter new data: ";
      getline(cin, newData);
      update(id, newData, records, password);
    }
    break;
  }

  case 4: {
    for (auto &record : filterByTableID(records, Universal_TableID)) {
      // Update "Last read" only if the data is not encrypted
      if (record.encryptionType == "NONE") {
        updateLastRead(record.id, records);
      }

      cout << record.id << ": ";

      // Check if the data is encrypted and potentially tempered
      if (record.encryptionType != "NONE" && record.state == 2) {
        cout << "Encrypted data that might have been tampered with. Decryption "
                "is not possible.";
      } else if (record.encryptionType != "NONE") {
        cout << "Encrypted data";
      } else if (record.state == 2) {
        // If the state is 2, there might be a potential integrity issue
        cout << "[POTENTIALLY TAMPERED]: " << record.data;
      } else {
        cout << record.data;
      }

      cout << " (Created at: " << record.timestamp
           << ", Last modified: " << record.last_modified << ", Last read: ";
      if (record.last_read.empty()) {
        cout << "Never been read";
      } else {
        cout << record.last_read;
      }
      cout << ")" << endl;
    }
    break;
  }

  case 5: {
    string keyword;
    cout << "Enter keyword: ";
    cin >> keyword;
    for (const auto &record : filterByKeyword(
             filterByCreator(records, currentUser->username), keyword)) {
      cout << record.id << ": " << record.data
           << " (Created at: " << record.timestamp << ")" << endl;
    }
    break;
  }
  case 6: {
    string term;
    cout << "Enter term: ";
    cin >> term;
    int totalFrequency = 0;
    for (const auto &record : filterByCreator(records, currentUser->username)) {
      if (record.encryptionType != "NONE") {
        continue;
      }
      int frequency = countTermFrequency(record.data, term);
      totalFrequency += frequency;
      if (frequency > 0) {
        cout << "Record ID " << record.id << ": " << frequency << " occurrences"
             << endl;
      }
    }
    cout << "Total occurrences of term \"" << term << "\": " << totalFrequency
         << endl;
    break;
  }
  case 7: {
    {
      int sortChoice;
      cout << "Sort Menu\n1. Sort Records (A-Z, Z-A)\n2. Sort Records by "
              "ID (Min to Max, Max to Min)\nEnter your choice: ";
      cin >> sortChoice;

      switch (sortChoice) {
      case 1: {
        int order;
        cout << "Sort Records\n1. A-Z\n2. Z-A\nEnter your choice: ";
        cin >> order;
        sortRecords(records, order == 2);
        cout << "Records sorted." << endl;
        break;
      }
      case 2: {
        int order;
        cout << "Sort Records by ID\n1. Min to Max\n2. Max to Min\nEnter "
                "your choice: ";
        cin >> order;
        sortRecordsById(records, order == 2);
        cout << "Records sorted by ID." << endl;
        break;
      }
      default: {
        cout << "Invalid choice!" << endl;
        break;
      }
      }
      break;
    }
  }
  case 8: {
    saveRecords(records);
    outputTableToJson(records, Universal_TableName, *currentUser,
                      Universal_TableID);

    break;
  }
  case 9: {
    saveRecords(records);
    tableMenuFlag = true;
    break;
  }
  case 10: {
    saveRecords(records);
    tableMenuFlag = false;
    string tempUser =
        currentUser->username; // Saves current user name for exit message
    if (logout(currentUser)) {
      currentUser = nullptr; // Set currentUser to nullptr to indicate logout
      cout << "Goodbye, " << tempUser << endl;
      break; // break out of the inner while-loop if logout was successful
    }
    cout << "Log out was unsuccessful" << endl;
    break;
  }

  default: {
    cout << "Invalid choice!" << endl;
    break;
  }
  }
}

int main() {
  vector<Table> tables = loadTables();
  vector<Record> records = loadRecords();
  vector<User> users = loadUsers();

  if (!isManager("123", users)) {
    createUser("123", "123", false, users);
    createUser("321", "321", false, users);
    saveUsers(users);
  }

  User *currentUser = nullptr;
  Table *currentTable = nullptr;
  bool tableMenuFlag = false;
  bool programTerminated = false;
  bool userAgreement = getUserAgreement();

  bool logoutFlag = false; // initialize a new flag

  while (userAgreement && !programTerminated) {
    while (!currentUser && !programTerminated) {
      logoutFlag = false; // reset logout flag
      loginMenu(currentUser, tableMenuFlag, users, programTerminated);
    }
    while (currentUser && !logoutFlag) {
      tableMenu(currentUser, tableMenuFlag, tables, currentTable, records,
                users, logoutFlag);
      if (!logoutFlag) {
        while (!tableMenuFlag && currentUser) {
          recordMenu(currentUser, tableMenuFlag, tables, currentTable, records,
                     users);
        }
      }
    }
  }

  return 0;
}
