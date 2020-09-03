#ifndef DB_HANDLER_H
#define DB_HANDLER_H

#include <sqlite3.h>
#include <string>
#include <queue>
#include "html_document.hpp"
using namespace std;

static int init_callback(void *NotUsed, int argc, char **argv, char **azColName);
static int insert_callback(void *NotUsed, int argc, char **argv, char **azColName);
static int select_callback(void *NotUsed, int argc, char **argv, char **azColName); 

class DbHandler
{
private:
  sqlite3 *db_reference;
  int insert_data(string word, string url, int weight);
  int insert_data(queue<string> words, string url, int weight);
  void insert_html_tag_data(HtmlDocument *doc, string tag, int weight);
public:
  int init_db();
  DbHandler(string db_file_path);
  ~DbHandler();
  int get_data(string word);
  void insert_html_document_data(HtmlDocument *doc);
};


#endif