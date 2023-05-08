#include "database.h"
#include "table.h"
#include <fstream>
#include <iomanip>
#include <iostream>

int main() {
  std::fstream file;
  file.open("info.csv", std::ios::in);
  if (!file.is_open()) {
    std::cout << "Cannot find info about saved databases and tables"
              << std::endl;
  }

  std::string s;
  std::vector<Database> databases;
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
    Database newDB(temp_db_name);
    for (int j = 0; j < tables.size(); j++) {
      if (tables[j] != "")
        newDB.createTable(tables[j]);
    }
    databases.push_back(newDB);
  }
  file.close();

  std::cout << "Welcome to YelaDB (v0.01 alpha)" << std::endl;

  int db = -1;
  int tbl = -1;
  while (true) {
    if (db == -1) {
      std::cout << "Choose database to work with:" << std::endl;
      std::cout << "0) Create new database" << std::endl;
      for (int i = 0; i < databases.size(); i++) {
        std::cout << i + 1 << ") " << databases[i].getName() << std::endl;
      }

      std::cout << databases.size() + 1 << ") Exit" << std::endl;
      
      std::cin >> db;
      while (db > databases.size() + 1 || db < 0) {
        std::cout << "Try again!" << std::endl;
        std::cin >> db;
      }

      if(db == databases.size() + 1){
        db = -1;
        break;
      }

      if (db == 0) {
        db = -1;
        std::cout << "Enter the name of database" << std::endl;
        std::string db_name;
        std::cin >> db_name;

        bool repeat = false;

        for (int i = 0; i < databases.size(); i++) {
          if (db_name == databases[i].getName()) {
            repeat = true;
          }
        }

        while (db_name == "" || repeat) {
          std::cout << "Try again!" << std::endl;
          if (repeat)
            std::cout << "This name is already used. Choose other one!"
                      << std::endl;
          std::cin >> db_name;
          repeat = false;
          for (int i = 0; i < databases.size(); i++) {
            if (db_name == databases[i].getName()) {
              repeat = true;
            }
          }
        }

        Database newDB(db_name);
        databases.push_back(newDB);
      }
    } else {
      if (tbl == -1) {
        std::cout << "Choose table to work with:" << std::endl;
        std::cout << "0) Create new table" << std::endl;
        std::cout << "1) Change name of database" << std::endl;
        std::cout << "2) Delete database" << std::endl;
        std::vector<Table> tables = databases[db - 1].getAllTables();

        for (int i = 0; i < tables.size(); i++) {
          std::cout << i + 3 << ") " << tables[i].getName() << std::endl;
        }

        std::cout << tables.size() + 3 << ") Go back" << std::endl;
        
        std::cin >> tbl;
        while (tbl > tables.size() + 3 || tbl < 0) {
          std::cout << "Try again!" << std::endl;
          std::cin >> tbl;
        }

        if(tbl == tables.size() + 3){
          db = -1;
          tbl = -1;
        }

        if (tbl == 0) {
          tbl = -1;

          std::cout << "Enter the name of table" << std::endl;
          std::string tbl_name;
          std::cin >> tbl_name;

          bool repeat = false;

          for (int i = 0; i < tables.size(); i++) {
            if (tbl_name == tables[i].getName()) {
              repeat = true;
            }
          }

          while (tbl_name == "" || repeat) {
            std::cout << "Try again!" << std::endl;
            if (repeat)
              std::cout << "This name is already used. Choose other one!"
                        << std::endl;
            std::cin >> tbl_name;
            repeat = false;
            for (int i = 0; i < tables.size(); i++) {
              if (tbl_name == tables[i].getName()) {
                repeat = true;
              }
            }
          }

          std::cout
              << "Enter names of columns of the table seperated by comma (,) (i.e id,name,surname)"
              << std::endl;
          std::vector<std::string> columns;
          std::string s;
          std::cin.ignore();
          getline(std::cin, s);

          for(int i = 0; i < s.size(); i++){
            std::string col;
            for(int j = i; j < s.size() && s[j] != ','; i++,j++){
              col.push_back(s[j]);
            }
            columns.push_back(col);
          }

          databases[db-1].createTable(tbl_name, columns);
        } else if(tbl == 1){
          tbl = -1;
          std::cout << "Enter new name of database" << std::endl;
          std::string newDBName;
          std::cin >> newDBName;
          bool repeat = false;
          for(int i = 0; i < databases.size(); i++){
            if(databases[i].getName() == newDBName){
              repeat = true;
            }
          }

          while(newDBName == "" || repeat){
            std::cout << "Try again!" << std::endl;
            std::cin >> newDBName;
            repeat = false;
            for(int i = 0; i < databases.size(); i++){
              if(databases[i].getName() == newDBName){
                repeat = true;
              }
            }
          }

          databases[db-1].changeName(newDBName);
        } else if(tbl == 2){
          tbl = -1;
          databases[db-1].deleteDatabase();
          databases.erase(databases.begin() + db-1);
          db = -1;
        }
      } else {
        std::cout << "Choose what action you want to perform" << std::endl;
        std::cout << "1) Insert data" << std::endl;
        std::cout << "2) See all data" << std::endl;
        std::cout << "3) Change data (you will need to provide row number)" << std::endl;
        std::cout << "4) Delete data (you will need to provide row number)" << std::endl;
        std::cout << "5) Change name of table" << std::endl;
        std::cout << "6) Delete table" << std::endl;
        std::cout << "7) Go back" << std::endl;

        int tbl_action = -1;
        std::cin >> tbl_action;

        while(tbl_action < 1 || tbl_action > 7){
          std::cout << "Try again!" << std::endl;
          std::cin >> tbl_action;
        }

        if(tbl_action == 7){
          tbl = -1;
        }

        if(tbl_action == 6){
          // std::cout << "deleting" << std::endl;
          databases[db-1].deleteTable(tbl-2);
          tbl = -1;
        }

        if(tbl_action == 5){
          std::cout << "Enter new name of the table" << std::endl;

          std::vector<Table> tables = databases[db-1].getAllTables();
          
          std::string tbl_name;
          std::cin >> tbl_name;

          bool repeat = false;

          for (int i = 0; i < tables.size(); i++) {
            if (tbl_name == tables[i].getName()) {
              repeat = true;
            }
          }

          while (tbl_name == "" || repeat) {
            std::cout << "Try again!" << std::endl;
            if (repeat)
              std::cout << "This name is already used. Choose other one!"
                        << std::endl;
            std::cin >> tbl_name;
            repeat = false;
            for (int i = 0; i < tables.size(); i++) {
              if (tbl_name == tables[i].getName()) {
                repeat = true;
              }
            }
          }

          databases[db-1].changeTableName(tbl-3,tbl_name);
        }

        if(tbl_action == 4){
          std::cout << "Enter the number of row, which you want to delete" << std::endl;
          
          int rowNum = -1;
          std::cin >> rowNum;

          if(rowNum < 1 || rowNum > databases[db-1].getNumRows(tbl-3)){
            std::cout << "Try again!" << std::endl;
            std::cin >> rowNum;
          }

          databases[db-1].deleteRowTable(rowNum, tbl-3);
        }

        if(tbl_action == 3){
          std::cout << "Enter the number of row, which you want to change" << std::endl;
          
          int rowNum = -1;
          std::cin >> rowNum;

          if(rowNum < 1 || rowNum > databases[db-1].getNumRows(tbl-3)){
            std::cout << "Try again!" << std::endl;
            std::cin >> rowNum;
          }

          std::vector<std::string> cols = databases[db-1].getColumns(tbl-3);
          std::vector<std::string> data(cols.size());
          for(int i = 0; i < cols.size(); i++){
            std::cout << cols[i] << ": ";
            std::cin >> data[i];
          }

          databases[db-1].updateRowTable(data, rowNum, tbl-3);
        }

        if(tbl_action == 2){
          std::vector<std::string> cols = databases[db-1].getColumns(tbl-3);                  
          std::vector<std::vector<std::string>> data = databases[db-1].getDataTable(tbl-3);

          for(int i = 0; i < cols.size(); i++){
            std::cout << std::left << std::setw(12) << cols[i];
          }

          std::cout << std::endl;

          for(int i = 0; i < data.size(); i++){
            for(int j = 0; j < data[i].size(); j++){
              std::cout << std::left << std::setw(12) << data[i][j];
            }
            std::cout << std::endl;
          }
        }

        if(tbl_action == 1){          
          std::vector<std::string> cols = databases[db-1].getColumns(tbl-3);                  
          std::vector<std::string> data(cols.size());
          for(int i = 0; i < cols.size(); i++){
            std::cout << cols[i] << ": ";
            std::cin >> data[i];
          }

          databases[db-1].insertToTable(data, tbl-3);
        }
      }
    }
  }

  return 0;
}