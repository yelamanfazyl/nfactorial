#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED

#include <string>
#include <vector>

class Table {
  std::string name;
  std::vector<std::string> columns;
  int col_num;
  std::string db_name;
  std::vector<std::vector<std::string>> data;

public:
  Table(std::string name, std::string db_name, bool &fine,
        std::vector<std::string> &columns);
  Table(std::string name, std::string db_name, bool &fine);

  void insert(std::vector<std::string> &data);
  std::vector<std::vector<std::string>> getAll();
  std::vector<std::string> getRow(int id);
  void updateRow(std::vector<std::string> &newData, int id);
  void deleteRow(int id);
  void deleteTable();
  void changeName(std::string name);
  void setDbName(std::string name);
  std::string getName();
  std::vector<std::string> getColumns();
  int getNumRows();
};

#endif