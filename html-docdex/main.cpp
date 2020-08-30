#include "html_document.hpp"
#include "stemmer.hpp"
#include "db_handler.hpp"
#include "html_handler.hpp"
#include <sqlite3.h>
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

int main() {
	sqlite3 *db_connection;
	HtmlDocument *doc;
  int error_status;
  DbHandler *db_handler = new DbHandler("prova.db");
  db_handler->init_db();

  string document_url = "https://www.dsi.unive.it/~salibra/";

	doc = new HtmlDocument(document_url);
	string html_string = doc->get_tag_content("body");
	cout << "tag content: " << html_string << endl;
	string stripped = HtmlHandler::strip_tags(html_string);

	cout << "stripped body: " << stripped << endl << endl;

	string stem = Stemmer::stem("yelling");
	cout << "Stemming: " << stem << endl;

	return 0;
}