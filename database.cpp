#include "database.h"
#include "table.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

bool checkIfDirExists(std::string name) {
  struct stat sb;

  return stat(name.c_str(), &sb) == 0;
}

Database::Database(std::string name) {
  if (!checkIfDirExists(name)) {
    mkdir(name.c_str(), 0777);
    std::fstream file;
    file.open("info.csv", std::ios::app);
    if (!file.is_open()) {
      std::cout << "Cannot open table" << std::endl;
      return;
    }

    file << name << std::endl;

    file.close();
  }

  this->name = name;
}

void Database::createTable(std::string name, std::vector<std::string> columns) {
  bool fine = true;
  Table table(name, this->name, fine, columns);
  if (!fine) {
    return;
  }

  this->tables.push_back(table);
}

// this function is for tables that are already created
void Database::createTable(std::string name) {
  bool fine = true;
  Table table(name, this->name, fine);
  if (!fine) {
    return;
  }

  this->tables.push_back(table);
}

Table* Database::chooseTable(int id) { return &(this->tables[id]); }

void Database::deleteTable(int id) {
  if (id - 1 >= tables.size() || id <= 0) {
    std::cout << "Cannot find that table" << std::endl;
    return;
  }

  this->tables[id - 1].deleteTable();

  this->tables.erase(this->tables.begin() + id - 1);
}

void Database::deleteDatabase() {
  std::error_code errorCode;
  if (!std::filesystem::remove_all(this->name.c_str(), errorCode)) {
    std::cout << errorCode.message() << std::endl;
  }

  std::fstream file;
  file.open("info.csv", std::ios::in);
  if (!file.is_open()) {
    std::cout << "Cannot save info about table" << std::endl;
    return;
  }

  std::vector<std::string> data;

  std::string s;

  while (getline(file, s)) {
    std::string temp_db_name = "";
    for (int i = 0; i < s.size() && s[i] != ','; i++) {
      temp_db_name.push_back(s[i]);
    }

    if (temp_db_name != this->name) {
      data.push_back(s);
    }
  }

  file.close();

  file.open("info.csv", std::ios::out);
  if (!file.is_open()) {
    std::cout << "Cannot save info about table" << std::endl;
    return;
  }

  for (int i = 0; i < data.size(); i++) {
    file << data[i] << std::endl;
  }

  file.close();
}

void Database::changeName(std::string name) {
  std::filesystem::rename(this->name.c_str(), name.c_str());
  for (int i = 0; i < tables.size(); i++) {
    tables[i].setDbName(this->name);
  }

  std::fstream file;
  file.open("info.csv", std::ios::in);
  if (!file.is_open()) {
    std::cout << "Cannot open file with data about database" << std::endl;
    return;
  }

  std::vector<std::string> data;

  std::string s;

  bool found = false;
  while (getline(file, s)) {
    std::string temp_db_name = "";
    int i = 0;
    for (; i < s.size() && s[i] != ','; i++) {
      temp_db_name.push_back(s[i]);
    }

    std::vector<std::string> tables;

    for (; i < s.size(); i++) {
      std::string table;
      for (int j = i; j < s.size() && s[j] != ','; j++, i++) {
        table.push_back(s[j]);
      }
      tables.push_back(table);
    }

    if (temp_db_name == this->name) {
      std::string newS = name;
      for (int j = 0; j < tables.size(); j++) {
        if (tables[j] != "")
          newS += "," + tables[j];
      }

      s = newS;
      found = true;
    }

    data.push_back(s);
  }

  file.close();

  if (!found) {
    std::cout << "Couldn't find info about database" << std::endl;
    return;
  }

  file.open("info.csv", std::ios::out);
  if (!file.is_open()) {
    std::cout << "Cannot save info about database" << std::endl;
    return;
  }

  for (int j = 0; j < data.size(); j++) {
    file << data[j] << std::endl;
  }

  file.close();
  this->name = name;
}

std::vector<Table> Database::getAllTables() { return this->tables; }

std::string Database::getName() { return this->name; }

void Database::insertToTable(std::vector<std::string> &data, int id){
  this->tables[id].insert(data);
}

void Database::updateRowTable(std::vector<std::string> &newData, int row, int id){
  this->tables[id].updateRow(newData, row);
}

void Database::deleteRowTable(int row, int id){
  this->tables[id].deleteRow(row);
}

std::vector<std::vector<std::string>> Database::getDataTable(int id){
  return this->tables[id].getAll();
}

void Database::changeTableName(int id, std::string name){
  this->tables[id].changeName(name);
}

int Database::getNumRows(int id){
  return this->tables[id].getNumRows();
}

std::vector<std::string> Database::getColumns(int id){
  return this->tables[id].getColumns();
}
