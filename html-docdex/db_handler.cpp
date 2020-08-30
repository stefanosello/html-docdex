#include "db_handler.hpp"
#include <sqlite3.h>
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

DbHandler::DbHandler(string db_file_path) {
  int error_status = sqlite3_open(db_file_path.c_str(), &(this->db_reference));
  if (error_status) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(this->db_reference));
  }
}

DbHandler::~DbHandler() {
  sqlite3_close(this->db_reference);
}

int DbHandler::init_db() {
  char *db_error_message = 0;
  int result;
  /* Create SQL statement */
  string sql = "CREATE TABLE WORDS("  \
        "id INT PRIMARY KEY," \
        "word           TEXT    NOT NULL," \
        "weight         INT     NOT NULL," \
        "website        TEXT );";
  result = sqlite3_exec(this->db_reference, sql.c_str(), init_callback, 0, &db_error_message);
  if( result != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", db_error_message);
    sqlite3_free(db_error_message);
  } else {
    fprintf(stdout, "Table created successfully\n");
  }
  return 0;
}

int DbHandler::insert_data(string word, string url, int weight) {
  return 0;
}

void DbHandler::get_data(string word) {

}

static int init_callback(void *NotUsed, int argc, char **argv, char **azColName) {
  int i;
  for(i = 0; i<argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}
