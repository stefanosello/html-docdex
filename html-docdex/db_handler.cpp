// -*- lsst-c++ -*-
/*
* db_handler.cpp is the definition file for the class DbHandler.
* It basically acts as a wrapper for the main db operations.
* Ideally, only one object of this class should be instatiated for a corresponding db file.
* The DBMS under the whole system is sqlite3, made available thanks to the official c/c++ library.
*/

#include "db_handler.hpp"
#include "html_document.hpp"
#include "html_handler.hpp"
#include "stemmer.hpp"
#include <sqlite3.h>
#include <iostream>
#include <stdio.h>
#include <regex>
#include <string>
#include <vector>
using namespace std;

/**
* Callback for db initiation query.
*/
static int init_callback(void *NotUsed, int argc, char **argv, char **azColName) {
  return 0;
}

/**
* Callback for db insert query.
*/
static int insert_callback(void *NotUsed, int argc, char **argv, char **azColName) {
  return 0;
}

/**
* Callback for db select query.
* It prints the websites corrisponding to the searched keyword given by the user.
*
* @param data Pointer to the data container
* @param argc The number of rows in the select query response
* @params argv The actual response rows
* @params azColName Array of cstrings containing the columns name
* @returns always 0 (there are no particular cases to handle)
*/
static int select_callback(void* data, int argc, char** argv, char** azColName) { 
  int i;
  for (i = 0; i < argc; i++) { 
    cout << argv[i] << endl;
  } 
  return 0; 
}

/**
* Constructor of the class.
* Creates the database file if it does not exists or opens it if already present.
*
* @param data Path of the db file
* @returns nothing
*/
DbHandler::DbHandler(string db_file_path) {
  int error_status = sqlite3_open(db_file_path.c_str(), &(this->db_reference));
  if (error_status) {
    fprintf(stderr, "Can't open database: %s \n", sqlite3_errmsg(this->db_reference));
  }
}

/**
* Distructor of the class.
* Closes the connection with the db.
*
* @returns nothing
*/
DbHandler::~DbHandler() {
  sqlite3_close(this->db_reference);
}

/**
* Db initiator.
* Executes the query needed to create the table and the relative indexes.
*
* @returns 0 if the query succeedes, -1 otherwise.
*/
int DbHandler::init_db() {
  char *db_error_message = 0;
  int result;

  string sql_create = "CREATE TABLE words(id INT PRIMARY KEY, word TEXT NOT NULL, weight INT NOT NULL, website TEXT NOT NULL);";
  string sql_index_1 = "CREATE INDEX word_idx ON words(word);";
  string sql_index_2 = "CREATE UNIQUE INDEX word_website_idx ON words(word, website);";
  string sql = sql_create + sql_index_1 + sql_index_2;

  result = sqlite3_exec(this->db_reference, sql.c_str(), init_callback, 0, &db_error_message);
  
  if( result != SQLITE_OK ){
    sqlite3_free(db_error_message);
    return -1;
  } else {
    return 0;
  }
}

/**
* Method to insert a single record in the words table.
* Not used actually.
* 
* @param word The word to insert
* @param url The url of the website containing that word
* @param weight The weight corrisponding to the word (relatively to the tag the word was contained into)
* @returns always 0 (next implementation could handle errors).
*/
int DbHandler::insert_data(string word, string url, int weight) {
  char *db_error_message = 0;
  string sql_insert = "INSERT OR IGNORE INTO words (word, weight, website) VALUES ('" + word + "'," + to_string(weight) + ", '" + url + "')";
  int result = sqlite3_exec(this->db_reference, sql_insert.c_str(), insert_callback, 0, &db_error_message);
  return 0;
}

/**
* Method to insert more records with a single query in the words table.
* 
* @param words A vector of string containing the words to insert
* @param url The url of the website containing those words
* @param weight The weight corrisponding to the words (relatively to the tag the words was contained into)
* @returns the query result code.
*/
int DbHandler::insert_data(vector<string> words, string url, int weight) {
  char *db_error_message = 0;
  int i = 0;
  string sql_insert = "INSERT OR IGNORE INTO words (word, weight, website) VALUES ";
  for_each(words.begin(), words.end(), [&](string word) {
    sql_insert += "('" + word + "'," + to_string(weight) + ", '" + url + "')";
    if (i < (words.size() - 1)) {
      sql_insert += ", ";
    }
    i++;
  });

  int result = sqlite3_exec(this->db_reference, sql_insert.c_str(), insert_callback, 0, &db_error_message);
  return result;
}

/**
* Get records from the words table starting from a keyword.
* 
* @param word The keyword to search for
* @returns the query result code.
*/
int DbHandler::get_data(string word) {
  char *db_error_message = 0;
  string sql_select = "SELECT DISTINCT website FROM words WHERE word = '" + Stemmer::stem(word) + "' LIKE word ORDER BY weight ASC;";
  int result = sqlite3_exec(this->db_reference, sql_select.c_str(), select_callback, 0, &db_error_message);
  return result;
}

/**
* Inserts words in the db (calling insert_data) starting from an HtmlDocument object.
* Acts also stemming those words by the Stemmer utility.
* Takes only those words contained in the specified tag, applying the specified weight.
* 
* @param doc The HtmlDocument object representing the website to index
* @param tag The tag to consider
* @param weight The weight assigned to the tag
* @returns the query result code.
*/
int DbHandler::insert_html_tag_data(HtmlDocument *doc, string tag, int weight) {
  size_t pos;
  string token;
  bool skip = false;
  vector<string> terms;
  string html_string;
  string delimiter = "_";

  regex substitution_regex("([^a-zA-Z0-9_]+)");
  html_string = doc->get_tag_content(tag);

  string stripped = HtmlHandler::strip_tags(html_string);

  for(string::iterator i = stripped.begin(); i != stripped.end(); i++) {
    char c = stripped.at(i - stripped.begin());
    if(!isalnum(c)) {
      if (i == stripped.begin() || stripped.at(i - stripped.begin() - 1) == '_') {
        stripped.erase(i);
      } else {
        stripped.replace(i - stripped.begin(), 1, delimiter);
      }
    }
  }

  while ((pos = stripped.find(delimiter)) != string::npos) {
    token = stripped.substr(0, pos);
    if (token.length() > 0) {
      token.erase(remove_if(token.begin(), token.end(), []( char const& c ) -> bool { return !isalnum(c); } ), token.end());
      if (token.length() > 3) {
        terms.push_back(token);
      }
    }
    stripped.erase(0, pos + delimiter.length());
  }

  for_each(terms.begin(), terms.end(), [](string s){
    s = Stemmer::stem(s);
  });

  return this->insert_data(terms, doc->get_url(), weight);
}

/**
* Inserts all website's words in the db, starting from an HtmlDocument object.
* It defines the weights of each tag.
* 
* @param doc The HtmlDocument object representing the website to index
* @returns nothing.
*/
void DbHandler::insert_html_document_data(HtmlDocument *doc) {
  
  this->insert_html_tag_data(doc, "title", 1);
  this->insert_html_tag_data(doc, "h1", 2);
  this->insert_html_tag_data(doc, "h2", 3);
  this->insert_html_tag_data(doc, "li", 4);
  this->insert_html_tag_data(doc, "a", 5);
  this->insert_html_tag_data(doc, "body", 6);

} 
