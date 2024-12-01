#include "business_layer_records.h"
#include "data_layer_records.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

/*
  Function: insert
  Desc: A function for the INSERT SQL command: inserts a record into a table
  Parameters: data to insert, the user info, and the database to insert in
  Return: the id of the record
*/
int insert(const string &data, const string &createdBy, vector<Record> &records,
           const int encryption, const string &cryptKey, const int tableID,
           const string &TableName) {
  Record newRecord;
  newRecord.id = getNextId(records);
  // kevin is working on this
  newRecord.data = data;
  newRecord.creator = createdBy;
  newRecord.timestamp = currentDateTime();
  newRecord.last_modified = currentDateTime();
  newRecord.last_read = "";
  newRecord.encryptionType = "NONE";
  newRecord.signature =
      generateSignature(data);     // This would keep the unencrypted signature
  newRecord.tableID = tableID;     // CW
  newRecord.tableName = TableName; // CW
  if (encryption == 1) {
    newRecord.data = encryptXOR(newRecord.data, cryptKey);
    newRecord.encryptionType = "XOR";
  }
  // KL, 05/11/2023
  if (encryption == 2) {
    newRecord.data = encryptXOR(newRecord.data, cryptKey);
    newRecord.encryptionType = "TEA";
  }
  records.push_back(newRecord);
  return newRecord.id;
}

/*
  Function: delete Record
  Desc: A function for the DELETE SQL command: deletes a record from a table
  Parameters: the id of the record to delete, the table that the record is
   located in
*/
void deleteRecord(int id, vector<Record> &records, const string &password) {
  auto it = find_if(records.begin(), records.end(),
                    [id](const Record &record) { return record.id == id; });

  if (it != records.end()) {
    if (it->encryptionType != "NONE") {
      string decryptedData = decryptXOR(it->data, password);
      // KL, 06/02/2023
      /*cout << "from deleteRecord"<<endl;
      cout << generateSignature(decryptedData) <<endl;
      cout << it->signature << endl;*/

      if (generateSignature(decryptedData) != it->signature) {
        cout << "decryptedData signature unmatch!";
        //<< generateSignature(decryptedData) << endl;  Debug MSG
        cout << "Invalid decryption key." << endl;
        return;
      }
    }
    it->last_modified = currentDateTime();
    records.erase(it);
  }
}

/*
  Function: update
  Desc: A function for the INSERT SQL command
  Parameters: id of record to update, the data to update, and the database of
   the record
*/
// Update to hanlde wrong ID input
void update(int id, const string &newData, vector<Record> &records,
            const string &password) {
  auto it = find_if(records.begin(), records.end(),
                    [id](const Record &record) { return record.id == id; });
  if (it != records.end()) {
    if (it->encryptionType != "NONE") {
      string decryptedData = decryptXOR(it->data, password);
      if (generateSignature(decryptedData) != it->signature) {
        cout << "Invalid decryption key." << endl;
        return;
      }
      // Generate new signature before encrypting new data
      it->signature = generateSignature(newData); // New Signature
      it->data = encryptXOR(newData, password);
    } else {
      it->data = newData;
      it->signature =
          generateSignature(newData); // New Signature for unencrypted data
    }
    it->last_modified = currentDateTime();
  } else {
    cout << "No record found with id: " << id << endl;
  }
}

void updateLastRead(int id, vector<Record> &records) {
  auto it = find_if(records.begin(), records.end(),
                    [id](const Record &record) { return record.id == id; });
  if (it != records.end()) {
    it->last_read = currentDateTime();
  }
}

vector<Record> displayRecord(int id, const vector<Record> &records,
                             User *currentUser, const string &password) {
  vector<Record> temp;
  auto record = getRecordById(id, records);

  if (record.creator == currentUser->username || currentUser->isManager) {
    if (record.encryptionType == "XOR") {
      record.data = decryptXOR(record.data, password);
      if (generateSignature(record.data) != record.signature) {
        /*
        cout << "decryptedData signature unmatch:"
             << generateSignature(record.data) << endl;
        cout << "DEBUG MESSAGE: " << record.data << endl;
        */
        // cout << "Invalid decryption key." << endl;

        return temp;
      }
    }
    temp.push_back(record);
  }
  return temp;
}

Record getRecordById(int id, const vector<Record> &records) {
  auto temp = find_if(records.begin(), records.end(),
                      [id](const Record &record) { return record.id == id; });

  if (temp != records.end()) {
    return *temp;
  } else {
    cout << "Record with specified ID does not exist.\n";
    return Record{-1, /* and so on for other fields */};
  }
}

bool compareByData(const Record &a, const Record &b) { return a.data < b.data; }

bool compareById(const Record &a, const Record &b) { return a.id < b.id; }

void sortRecords(vector<Record> &records, bool reverse) {
  vector<Record> nonEncryptedRecords;
  vector<Record> encryptedRecords;

  // Split into 2 Vectors
  for (const auto &record : records) {
    if (record.encryptionType == "NONE") {
      nonEncryptedRecords.push_back(record);
    } else {
      encryptedRecords.push_back(record);
    }
  }

  // Sort them separately
  if (reverse) {
    sort(nonEncryptedRecords.rbegin(), nonEncryptedRecords.rend(),
         compareByData);
    sort(encryptedRecords.rbegin(), encryptedRecords.rend(), compareByData);
  } else {
    sort(nonEncryptedRecords.begin(), nonEncryptedRecords.end(), compareByData);
    sort(encryptedRecords.begin(), encryptedRecords.end(), compareByData);
  }

  // Joint them back
  nonEncryptedRecords.insert(nonEncryptedRecords.end(),
                             encryptedRecords.begin(), encryptedRecords.end());

  // Overwrite
  records = nonEncryptedRecords;
}

void sortRecordsById(vector<Record> &records, bool reverse) {
  if (reverse) {
    sort(records.rbegin(), records.rend(), compareById);
  } else {
    sort(records.begin(), records.end(), compareById);
  }
}

// New patches End

/*
flag: Controls the operation of the function. If it is 1, the function will just
delete the records. If it is 2, the function will delete the records and also
output all the records that have encryptionType = "NONE".
*/
std::pair<int, int> deleteTableRecords(const std::string &tableName,
                                       const int tableID,
                                       std::vector<Record> &records,
                                       const int flag) {
  int deletedRecords = 0;
  int deletedEncryptedRecords = 0;

  records.erase(
      remove_if(records.begin(), records.end(),
                [&](Record &record) {
                  if (record.tableID == tableID &&
                      record.tableName == tableName) {
                    if (record.encryptionType != "NONE") {
                      deletedEncryptedRecords++;
                    }

                    // If flag is 2, and record is not encrypted, output the
                    // record
                    if (flag == 2 && record.encryptionType == "NONE") {
                      cout << "ID: " << record.id << "\n";
                      cout << "Data: " << record.data << "\n";
                      cout << "Creator: " << record.creator << "\n";
                      cout << "=====================================\n";
                    }

                    deletedRecords++;
                    return true;
                  }
                  return false;
                }),
      records.end());

  if (flag == 2 && deletedEncryptedRecords > 0) {
    cout << deletedEncryptedRecords
         << " encrypted records have been deleted but not shown.\n";
  }

  return make_pair(deletedRecords, deletedEncryptedRecords);
}

// The function below handle Encryption/Decryption
string encryptXOR(const string &password, const string &XOR_KEY) {
  if (XOR_KEY.empty()) {
    throw invalid_argument("XOR_KEY must not be empty.");
  }
  string encrypted_password = password;
  int key_length = XOR_KEY.length();
  for (int i = 0; i < encrypted_password.length(); i++) {
    encrypted_password[i] = encrypted_password[i] ^ XOR_KEY[i % key_length];
  }
  return encrypted_password;
}

string decryptXOR(const string &encrypted_password, const string &XOR_KEY) {
  if (XOR_KEY.empty()) {
    throw invalid_argument("XOR_KEY must not be empty.");
  }
  string password = encrypted_password;
  int key_length = XOR_KEY.length();
  for (int i = 0; i < password.length(); i++) {
    password[i] = password[i] ^ XOR_KEY[i % key_length];
  }
  return password;
}

///
/// TEA ENCRYPTION
/// HP 05/11/2023

vector<unsigned int> encryptTEA(const vector<unsigned int> &password,
                                const vector<unsigned int> TEAKEY) {
  unsigned int p0 = password[0], p1 = password[1], sum = 0;
  unsigned int delta = 0x9e3779b9;
  unsigned int k0 = TEAKEY[0], k1 = TEAKEY[1], k2 = TEAKEY[2], k3 = TEAKEY[3];

  for (int i = 0; i < 32; ++i) {
    sum += delta;
    p0 += ((p1 << 4) + k0) ^ (p1 + sum) ^ ((p1 >> 5) + k1);
    p1 += ((p0 << 4) + k2) ^ (p0 + sum) ^ ((p0 >> 5) + k3);
  }

  vector<unsigned int> encrypted_password(2);
  encrypted_password[0] = p0;
  encrypted_password[1] = p1;

  return encrypted_password;
}

vector<unsigned int> decryptTEA(const vector<unsigned int> &encrypted_password,
                                const vector<unsigned int> TEAKEY) {
  unsigned int p0 = encrypted_password[0], p1 = encrypted_password[1],
               sum = 0xC6EF3720, i;
  unsigned int delta = 0x9e3779b9;
  unsigned int k0 = TEAKEY[0], k1 = TEAKEY[1], k2 = TEAKEY[2], k3 = TEAKEY[3];
  for (i = 0; i < 32; i++) {
    p1 -= ((p0 << 4) + k2) ^ (p0 + sum) ^ ((p0 >> 5) + k3);
    p0 -= ((p1 << 4) + k0) ^ (p1 + sum) ^ ((p1 >> 5) + k1);
    sum -= delta;
  }

  vector<unsigned int> password(2);
  password[0] = p0;
  password[1] = p1;

  return password;
}

string generateSignature(const string &str) {
  hash<string> hasher;
  size_t hash = hasher(str);
  return to_string(hash);
}

string getDecryptKey() { // Helper
  string key;
  cout << "Enter the decryption key: ";
  getline(cin, key);
  return key;
}

/// Reorder, the following functions would skip encrypted data when doing DB
/// query

/*
  Function: filterByCreator
  Desc: Searches the database table for records that were created by a
    specific user Parameters: the database table to parse and the user to
  search for Return: all the filtered records in the database belonging to the
  specified user
*/
vector<Record> filterByCreator(const vector<Record> &records,
                               const string &createdBy) {
  vector<Record> filteredRecords;
  for (const auto &record : records) {
    if (record.creator == createdBy) {
      filteredRecords.push_back(record);
    }
  }
  return filteredRecords;
}

/*
NEW FUNCTION
*/
vector<Record> filterByTableID(const vector<Record> &records, int tableID) {
  vector<Record> filteredRecords;
  for (const auto &record : records) {
    if (record.tableID == tableID) {
      filteredRecords.push_back(record);
    }
  }
  return filteredRecords;
}

/*
  Function: countTermFrequency
  Desc: counts the frequency of a specific term in the data
  Parameters: the data of the record and the term to search
  Return: the number of terms counted
*/
int countTermFrequency(const string &data, const string &term) {
  int count = 0;
  size_t pos = 0;
  while ((pos = data.find(term, pos)) != string::npos) {
    count++;
    pos += term.length();
  }
  return count;
}

/*
  Function: filterByKeyword
  Desc: Searches the database table for a specific keyword and returns those
    records that match Parameters: the database table to parse and the keyword
  to search Return: all the filtered records in the database table pertaining
  to the keyword
        Updated 5/29/23 to skip encrypted data.
*/
vector<Record> filterByKeyword(const vector<Record> &records,
                               const string &keyword) {
  vector<Record> filteredRecords;
  for (const auto &record : records) {
    // If the data is not encrypted, perform the search
    if (record.encryptionType == "NONE" &&
        record.data.find(keyword) != string::npos) {
      filteredRecords.push_back(record);
    }
  }
  return filteredRecords;
}