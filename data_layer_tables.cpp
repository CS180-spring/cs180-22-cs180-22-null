#include "data_layer_tables.h"
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

string generateSignature(const Table &table) {
  hash<string> hasher;
  size_t hash = hasher(to_string(table.id) + table.name + table.owner);
  return to_string(hash);
}

void saveTables(const vector<Table> &tables) {
  json j_tables = json::array();

  for (const Table &table : tables) {
    string signature = generateSignature(table);
    json j_table = {{"id", table.id},
                    {"owner", table.owner},
                    {"name", table.name},
                    {"authorizedCollaborators", table.authorizedCollaborators},
                    {"signature", signature}};
    j_tables.push_back(j_table);
  }

  ofstream ofs("tables.json");
  ofs << j_tables.dump(4) << endl;
  ofs.close();
}

vector<Table> loadTables() {
  vector<Table> tables;

  ifstream ifs("tables.json");
  if (ifs.is_open()) {
    json j_tables;
    ifs >> j_tables;

    for (const auto &j_table : j_tables) {
      Table table;
      table.id = j_table["id"];
      table.owner = j_table["owner"];
      table.name = j_table["name"];
      table.authorizedCollaborators =
          j_table["authorizedCollaborators"].get<std::vector<int>>();
      string storedSignature = j_table["signature"];

      // Generate a signature based on the loaded table and compare it to the
      // stored signature
      string generatedSignature = generateSignature(table);
      if (storedSignature != generatedSignature) {
        // The signature doesn't match - the table may have been tampered with!
        throw runtime_error("Table signature doesn't match!");
      }

      tables.push_back(table);
    }

    ifs.close();
  }

  return tables;
}

int getNextID(const vector<Table> &tables) {
  int maxId = 0;
  for (const auto &table : tables) {
    if (table.id > maxId) {
      maxId = table.id;
    }
  }
  return maxId + 1;
}