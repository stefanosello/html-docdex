#ifndef DB_HANDLER_H
#define DB_HANDLER_H

#include <sqlite3.h>
#include <string>
using namespace std;

static int init_callback(void *NotUsed, int argc, char **argv, char **azColName);

class DbHandler
{
private:
  sqlite3 *db_reference;
public:
  DbHandler(string db_file_path);
  ~DbHandler();
  int init_db();
  int insert_data(string word, string url, int weight);
  void get_data(string word);
};


#endif