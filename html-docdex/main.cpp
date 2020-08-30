#include "html_document.hpp"
#include "stemmer.hpp"
#include <sqlite3.h>
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

int main() {
	sqlite3 *db_connection;
	HtmlDocument *doc;
  int error_status;
  std::string title;

  string document_url = "https://en.wikipedia.org/wiki/Alcide_De_Gasperi";
	
	error_status = sqlite3_open("prova.db", &db_connection);
	if (error_status) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db_connection));
		return 1;
	}
	std::cout << "Successfully opened sqlite db!" << endl;	
	sqlite3_close(db_connection);

	doc = new HtmlDocument(document_url);
	//title = doc->get_tag_content("");

	//cout << title << endl;

	string stem = Stemmer::stem("demonstrating");

	cout << "Stemming: " << stem << endl;

	return 0;
}