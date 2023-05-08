#include "table.h"
#include <filesystem>
#include <fstream>
#include <iostream>

Table::Table(std::string name, std::string db_name, bool &fine) {
  std::fstream file;
  file.open(db_name + '/' + name + ".csv", std::ios::in);
  if (!file.is_open()) {
    std::cout << "Can't find this table" << std::endl;
    fine = false;
    return;
  }

  std::string s;
  getline(file, s);

  std::vector<std::string> columns;

  for (int i = 0; i < s.size(); i++) {
    std::string column;
    for (int j = i; j < s.size() && s[j] != ','; j++, i++) {
      column.push_back(s[j]);
    }

    if (column != "") {
      columns.push_back(column);
    }
  }

  this->columns = columns;
  this->col_num = columns.size();
  this->name = name;

  std::vector<std::vector<std::string>> data;

  while (getline(file, s)) {
    std::vector<std::string> row;

    for (int i = 0; i < s.size(); i++) {
      std::string piece = "";
      for (int j = i; j < s.size() && s[j] != ','; j++, i++) {
        piece.push_back(s[j]);
      }

      if (piece != "") {
        row.push_back(piece);
      }
    }

    data.push_back(row);
  }

  file.close();

  this->data = data;
  this->db_name = db_name;
}

Table::Table(std::string name, std::string db_name, bool &fine,
             std::vector<std::string> &columns) {
  std::fstream file;
  file.open(db_name + '/' + name + ".csv", std::ios::out);
  if (!file.is_open()) {
    std::cout << "Cannot create table" << std::endl;
    fine = false;
    return;
  }
  for (int i = 0; i < columns.size() - 1; i++) {
    file << columns[i] << ",";
  }
  file << columns[columns.size() - 1] << std::endl;
  file.close();
  this->name = name;
  this->db_name = db_name;
  this->columns = columns;
  col_num = columns.size();

  file.open("info.csv", std::ios::in);
  if (!file.is_open()) {
    std::cout << "Cannot save info about table" << std::endl;
    fine = false;
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

    if (temp_db_name == db_name) {
      bool found_table = false;
      for (int j = 0; j < tables.size(); j++) {
        if (tables[j] == name) {
          found_table = true;
        }
      }
      if (!found_table)
        s += "," + name;

      found = true;
    }

    data.push_back(s);
  }

  if (!found) {
    std::cout << "Couldn't find info about database123123" << std::endl;
    fine = false;
    return;
  }

  file.close();

  file.open("info.csv", std::ios::out);
  if (!file.is_open()) {
    std::cout << "Cannot save info about table" << std::endl;
    fine = false;
    return;
  }

  for (int j = 0; j < data.size(); j++) {
    file << data[j] << std::endl;
  }

  file.close();
}

void Table::insert(std::vector<std::string> &data) {
  if (data.size() != col_num) {
    std::cout << "Number of columns doesn't match with number of entries"
              << std::endl;
    return;
  }
  
  this->data.push_back(data);

  std::fstream file;
  file.open(db_name + '/' + name + ".csv", std::ios::app);
  if (!file.is_open()) {
    std::cout << "Cannot open table" << std::endl;
    return;
  }

  for (int i = 0; i < data.size() - 1; i++) {
    file << data[i] << ",";
  }
  file << data[data.size() - 1] << std::endl;

  file.close();
}

std::vector<std::vector<std::string>> Table::getAll() { return this->data; }

std::vector<std::string> Table::getRow(int id) { return this->data[id - 1]; }

void Table::updateRow(std::vector<std::string> &newData, int id) {
  if (id - 1 >= this->data.size() || id <= 0) {
    std::cout << "Cannot find this row" << std::endl;
    return;
  }

  if (newData.size() != this->col_num) {
    std::cout << "New data's number of columns do not match with tables";
    return;
  }

  this->data[id - 1] = newData;

  std::fstream file;
  file.open(db_name + '/' + name + ".csv", std::ios::out);
  if (!file.is_open()) {
    std::cout << "Cannot open table" << std::endl;
    return;
  }

  for (int i = 0; i < columns.size() - 1; i++) {
    file << columns[i] << ",";
  }
  file << columns[columns.size() - 1] << std::endl;

  for (int i = 0; i < this->data.size(); i++) {
    for (int j = 0; j < this->data[i].size() - 1; j++) {
      file << this->data[i][j] + ",";
    }
    file << this->data[i][this->data[i].size() - 1] << std::endl;
  }

  file.close();
}

void Table::deleteRow(int id) {
  if (id - 1 >= this->data.size() || id <= 0) {
    return;
  }

  this->data.erase(this->data.begin() + id - 1);

  std::fstream file;

  file.open(db_name + '/' + name + ".csv", std::ios::out);
  if (!file.is_open()) {
    std::cout << "Cannot open table" << std::endl;
    return;
  }

  for (int i = 0; i < columns.size() - 1; i++) {
    file << columns[i] << ",";
  }
  file << columns[columns.size() - 1] << std::endl;

  for (int i = 0; i < this->data.size(); i++) {
    for (int j = 0; j < this->data[i].size() - 1; j++) {
      file << this->data[i][j] + ",";
    }
    file << this->data[i][this->data[i].size() - 1] << std::endl;
  }

  file.close();
}

void Table::deleteTable() {
  std::fstream file;
  file.open("info.csv", std::ios::in);
  if (!file.is_open()) {
    std::cout << "Cannot save info about table" << std::endl;
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

    // std::cout << temp_db_name << "::::::::" << db_name << std::endl;

    if (temp_db_name == db_name) {
      bool found_table = false;
      int tbl_idx = -1;
      for (int j = 0; j < tables.size(); j++) {
        if (tables[j] == this->name) {
          found_table = true;
          tbl_idx = j;
        }
      }

      if (!found_table) {
        std::cout << "Didn't find info about this table" << std::endl;
        return;
      }

      tables.erase(tables.begin() + tbl_idx);

      std::string newS = temp_db_name;

      for (int j = 0; j < tables.size(); j++) {
        if (tables[j] != "")
          newS += "," + tables[j];
      }

      s = newS;

      found = true;
    }

    data.push_back(s);
  }

  if (!found) {
    std::cout << "Couldn't find info about database" << std::endl;
    return;
  }

  file.close();

  file.open("info.csv", std::ios::out);
  if (!file.is_open()) {
    std::cout << "Cannot save info about table" << std::endl;
    return;
  }

  for (int j = 0; j < data.size(); j++) {
    file << data[j] << std::endl;
  }

  file.close();

  std::string path = db_name + '/' + name + ".csv";
  std::remove(path.c_str());
}

void Table::setDbName(std::string name) { this->db_name = name; }

void Table::changeName(std::string name) {
  std::string oldPath = db_name + '/' + this->name + ".csv";
  std::string newPath = db_name + '/' + name + ".csv";
  std::filesystem::rename(oldPath.c_str(), newPath.c_str());

  std::fstream file;
  file.open("info.csv", std::ios::in);
  if (!file.is_open()) {
    std::cout << "Cannot save info about table" << std::endl;
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

    if (temp_db_name == db_name) {
      bool found_table = false;
      for (int j = 0; j < tables.size(); j++) {
        if (tables[j] == this->name) {
          found_table = true;
          tables[j] = name;
        }
      }

      if (!found_table) {
        std::cout << "Didn't find info about this table" << std::endl;
        return;
      }

      std::string newS = temp_db_name;

      for (int j = 0; j < tables.size(); j++) {
        if (tables[j] != "")
          newS += "," + tables[j];
      }

      s = newS;

      found = true;
    }

    data.push_back(s);
  }

  if (!found) {
    std::cout << "Couldn't find info about database" << std::endl;
    return;
  }

  file.close();

  file.open("info.csv", std::ios::out);
  if (!file.is_open()) {
    std::cout << "Cannot save info about table" << std::endl;
    return;
  }

  for (int j = 0; j < data.size(); j++) {
    file << data[j] << std::endl;
  }

  file.close();

  this->name = name;
}

std::string Table::getName() { return this->name; }
std::vector<std::string> Table::getColumns() { return this->columns; }
int Table::getNumRows() { return this->data.size(); }