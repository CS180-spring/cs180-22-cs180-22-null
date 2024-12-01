#ifndef BUSINESS_LAYER_RECORDS_H
#define BUSINESS_LAYER_RECORDS_H

#include "business_layer_user.h"
#include <string>
#include <vector>

struct Record {
  int id;
  int tableID;
  std::string tableName;
  std::string data;
  std::string timestamp;
  std::string last_modified;
  std::string last_read;
  std::string creator;
  std::string encryptionType = "NONE";
  std::string signature;
  std::string jsonSignature;
	int state = 1;
};
std::string generateSignature(const std::string &str);
std::string encryptXOR(const std::string &password, const std::string &XORKEY);
std::string decryptXOR(const std::string &encrypted_password,
                       const std::string &XORKEY);
std::vector<unsigned int> encryptTEA(const std::vector<unsigned int> &password,
                                     const std::vector<unsigned int> TEAKEY);
std::vector<unsigned int>
decryptTEA(const std::vector<unsigned int> &encrypted_password,
           const std::vector<unsigned int> TEAKEY);

int getNextId(const std::vector<Record> &records);
std::string currentDateTime();

int insert(const std::string &data, const std::string &createdBy,
           std::vector<Record> &records, const int encryption,
           const std::string &cryptKey, const int tableID,
           const std::string &TableName);

void deleteRecord(int id, std::vector<Record> &records,
                  const std::string &password);
void update(int id, const std::string &newData, std::vector<Record> &records,
            const std::string &password);

std::vector<Record> filterByKeyword(const std::vector<Record> &records,
                                    const std::string &keyword);
std::vector<Record> filterByCreator(const std::vector<Record> &records,
                                    const std::string &createdBy);
std::vector<Record> filterByTableID(const std::vector<Record> &records,
                                    int tableID); // NEWLY ADDED
int countTermFrequency(const std::string &data, const std::string &term);

void updateLastRead(int id, std::vector<Record> &records);

Record getRecordById(int id, const std::vector<Record> &records);
std::vector<Record> displayRecord(int id, const std::vector<Record> &records,
                                  User *currentUser,
                                  const std::string &password);
void sortRecords(std::vector<Record> &records, bool reverse);
void sortRecordsById(std::vector<Record> &records, bool reverse);
bool compareByData(const Record &a, const Record &b);
bool compareById(const Record &a, const Record &b);
std::pair<int, int> deleteTableRecords(const std::string &tableName,
                                       const int tableID,
                                       std::vector<Record> &records,
                                       const int flag);

#endif // BUSINESS_LAYER_RECORDS_H
