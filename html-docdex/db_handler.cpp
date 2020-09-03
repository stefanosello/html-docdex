#include "db_handler.hpp"
#include "html_document.hpp"
#include "html_handler.hpp"
#include "stemmer.hpp"
#include <sqlite3.h>
#include <iostream>
#include <stdio.h>
#include <regex>
#include <string>
#include <queue>
using namespace std;

static int init_callback(void *NotUsed, int argc, char **argv, char **azColName) {
  return 0;
}

static int insert_callback(void *NotUsed, int argc, char **argv, char **azColName) {
  return 0;
}

static int select_callback(void* data, int argc, char** argv, char** azColName) 
{ 
  int i;

  for (i = 0; i < argc; i++) { 
    cout << argv[i] << endl;
  } 

  return 0; 
}

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

  /* Create SQL statements */
  string sql_create = "CREATE TABLE words(id INT PRIMARY KEY, word TEXT NOT NULL, weight INT NOT NULL, website TEXT NOT NULL);";
  string sql_index_1 = "CREATE INDEX word_idx ON words(word);";
  string sql_index_2 = "CREATE UNIQUE INDEX word_website_idx ON words(word, website);";
  string sql = sql_create + sql_index_1 + sql_index_2;

  result = sqlite3_exec(this->db_reference, sql.c_str(), init_callback, 0, &db_error_message);
  
  if( result != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", db_error_message);
    sqlite3_free(db_error_message);
    return -1;
  } else {
    fprintf(stdout, "Table created successfully\n");
    return 0;
  }
}

int DbHandler::insert_data(string word, string url, int weight) {
  char *db_error_message = 0;
  string sql_insert = "INSERT OR IGNORE INTO words (word, weight, website) VALUES ('" + word + "'," + to_string(weight) + ", '" + url + "')";
  int result = sqlite3_exec(this->db_reference, sql_insert.c_str(), insert_callback, 0, &db_error_message);
  return 0;
}

int DbHandler::insert_data(queue<string> words, string url, int weight) {
  char *db_error_message = 0;
  string sql_insert = "INSERT OR IGNORE INTO words (word, weight, website) VALUES ";
  while (!words.empty()) {
    string word = words.front();
    sql_insert += "('" + word + "'," + to_string(weight) + ", '" + url + "')";
    words.pop();
    if (!words.empty()) {
      sql_insert += ", ";
    }
  }
  int result = sqlite3_exec(this->db_reference, sql_insert.c_str(), insert_callback, 0, &db_error_message);
  return result;
}

int DbHandler::get_data(string word) {
  char *db_error_message = 0;
  string sql_select = "SELECT DISTINCT website FROM words WHERE word LIKE '" + Stemmer::stem(word) + "' ORDER BY weight ASC;";
  int result = sqlite3_exec(this->db_reference, sql_select.c_str(), select_callback, 0, &db_error_message);
  return result;
}

void DbHandler::insert_html_tag_data(HtmlDocument *doc, string tag, int weight) {
  size_t pos;
  string token;
  bool skip = false;
  queue<string> terms;
  string html_string;
  string delimiter = " ";
  regex substitution_regex("[^a-zA-Z0-9]*");

  try {
    html_string = doc->get_tag_content(tag);
  } catch (...) {
    cerr << "Tag content retrieving failed for tag " << tag << " and url " <<  doc->get_url();
    return;
  }

  string stripped = HtmlHandler::strip_tags(html_string);
  regex_replace(stripped, substitution_regex, " ");
  while ((pos = stripped.find(delimiter)) != string::npos) {
    token = stripped.substr(0, pos);
    if (token.length() > 0) {
      token.erase(remove_if(token.begin(), token.end(), []( auto const& c ) -> bool { return !isalnum(c); } ), token.end());
      if (token.length() > 3) {
        terms.push(token);
      }
    }
    stripped.erase(0, pos + delimiter.length());
  }

  this->insert_data(terms, doc->get_url(), weight);
}

void DbHandler::insert_html_document_data(HtmlDocument *doc) {
  this->insert_html_tag_data(doc, "title", 0);
  this->insert_html_tag_data(doc, "h1", 1);
  this->insert_html_tag_data(doc, "h2", 2);
  this->insert_html_tag_data(doc, "h3", 3);
  this->insert_html_tag_data(doc, "h4", 4);
  // this->insert_html_tag_data(doc, "b", 5);
  this->insert_html_tag_data(doc, "strong", 6);
  this->insert_html_tag_data(doc, "li", 7);
  this->insert_html_tag_data(doc, "p", 8);
  this->insert_html_tag_data(doc, "body", 9);
} 
