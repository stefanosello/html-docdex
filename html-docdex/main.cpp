#include "html_document.hpp"
#include "stemmer.hpp"
#include "db_handler.hpp"
#include "html_handler.hpp"
#include <sqlite3.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <regex>
using namespace std;

int main() {
	sqlite3 *db_connection;
	HtmlDocument *doc;
  int error_status;
  

  DbHandler *db_handler = new DbHandler("prova.db");
  db_handler->init_db();

  string document_url = "https://www.dsi.unive.it/~salibra/";

	doc = new HtmlDocument(document_url);
	db_handler->insert_html_document_data(doc);
	db_handler->get_data("torino");
	
	return 0;
}