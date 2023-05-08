#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED

#include <string>
#include <vector>
#include "table.h"

class Database{
  std::string name;
  std::vector<Table> tables;
  public:
    Database(std::string name);
    void createTable(std::string name, std::vector<std::string> columns);
    void createTable(std::string name);
    void deleteTable(int id);
    Table* chooseTable(int id);
    void deleteDatabase();
    void changeName(std::string name);
    std::vector<Table> getAllTables();
    std::string getName();
    void insertToTable(std::vector<std::string> &data, int id);
    void updateRowTable(std::vector<std::string> &newData, int row, int id);
    void deleteRowTable(int row, int id);
    std::vector<std::vector<std::string>> getDataTable(int id);
    void changeTableName(int id, std::string name);
    int getNumRows(int id);
    std::vector<std::string> getColumns(int id);
};

#endif