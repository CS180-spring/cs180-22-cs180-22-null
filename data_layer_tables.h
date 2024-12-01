#ifndef DATA_LAYER_TABLES_H
#define DATA_LAYER_TABLES_H

#include "business_layer_tables.h"
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

std::vector<Table> loadTables();
void saveTables(const std::vector<Table> &tables);
int getNextID(const std::vector<Table> &tables);

#endif // DATA_LAYER_TABLES_H