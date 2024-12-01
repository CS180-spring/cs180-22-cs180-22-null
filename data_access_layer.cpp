#include "data_access_layer.h"
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

using json = nlohmann::json;
using namespace std;

vector<Record> loadRecords() {
  vector<Record> records;

  ifstream ifs("records.json");
  if (ifs.is_open()) {
    json j_records;
    ifs >> j_records;

    for (const auto &j_record : j_records) {
      Record record;
      record.id = j_record["id"];
      record.data = j_record["data"];
      record.creator = j_record["creator"];
      record.timestamp = j_record["timestamp"];
      record.last_modified = j_record["last_modified"];
      record.last_read = j_record["last_read"];
      record.encryptionType = j_record["encryptionType"];
      record.signature = j_record["signature"];
      records.push_back(record);
    }

    ifs.close();
  }

  return records;
}

void saveRecords(const vector<Record> &records) {
  json j_records = json::array();

  for (const Record &record : records) {
    json j_record = {{"id", record.id},
                     {"data", record.data},
                     {"creator", record.creator},
                     {"timestamp", record.timestamp},
                     {"last_modified", record.last_modified},
                     {"last_read", record.last_read},
                     {"encryptionType", record.encryptionType},
                     {"signature", record.signature}};
    j_records.push_back(j_record);
  }

  ofstream ofs("records.json");
  ofs << j_records.dump(4) << endl;
  ofs.close();
}

// New Patches

// CHATGPT HELPS
string base64_encode(const string &in) {
  string out;

  int val = 0, valb = -6;
  for (unsigned char c : in) {
    val = (val << 8) + c;
    valb += 8;
    while (valb >= 0) {
      out.push_back(
          "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
              [(val >> valb) & 0x3F]);
      valb -= 6;
    }
  }
  if (valb > -6)
    out.push_back(
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
            [((val << 8) >> (valb + 8)) & 0x3F]);
  while (out.size() % 4)
    out.push_back('=');
  return out;
}

// CHATGPT HELPS
string base64_decode(const string &in) {
  string out;

  vector<int> T(256, -1);
  for (int i = 0; i < 64; i++)
    T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] =
        i;

  int val = 0, valb = -8;
  for (unsigned char c : in) {
    if (T[c] == -1)
      break;
    val = (val << 6) + T[c];
    valb += 6;
    if (valb >= 0) {
      out.push_back(char((val >> valb) & 0xFF));
      valb -= 8;
    }
  }
  return out;
}

// Lucky Number 8

string encryptString(const string &str) {
  string encrypted = base64_encode(str);
  for_each(encrypted.begin(), encrypted.end(),
           [](char &c) { c = c + 8; }); // Caesar Cipher
  return encrypted;
}

string decryptString(const string &str) {
  string decrypted = str;
  for_each(decrypted.begin(), decrypted.end(),
           [](char &c) { c = c - 8; }); // Caesar Cipher
  return base64_decode(decrypted);
}

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
    json j_user = {{"username", user.username},
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

// New Patches End

int getNextId(const vector<Record> &records) {
  int maxId = 0;
  for (const auto &record : records) {
    if (record.id > maxId) {
      maxId = record.id;
    }
  }
  return maxId + 1;
}

string currentDateTime() {
  time_t now = time(0);
  tm *localtm = localtime(&now);
  char buffer[80];
  strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtm);
  return string(buffer);
}
