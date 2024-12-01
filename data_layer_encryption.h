#ifndef DATA_LAYER_ENCRYPTION_H
#define DATA_LAYER_ENCRYPTION_H

#include "data_layer_records.h"
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

std::string base64_encode(const std::string &in);
std::string base64_decode(const std::string &in);
std::string encryptString(const std::string &str);
std::string decryptString(const std::string &str);

#endif // DATA_LAYER_ENCRYPTION_H