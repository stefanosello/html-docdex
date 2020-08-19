#include <iostream>
#include <stdio.h>
#include <sqlite3.h> 
#include "html_document.hpp"
using namespace std;

int main() {
	sqlite3 *db_connection;
	HtmlDocument *doc;
    int error_status;
    std::string title;
	
	error_status = sqlite3_open("prova.db", &db_connection);
	if (error_status) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db_connection));
		return 1;
	}
	std::cout << "Successfully opened sqlite db!\n";	
	sqlite3_close(db_connection);

	doc = new HtmlDocument("https://en.wikipedia.org/wiki/Alcide_De_Gasperi");
	title = doc->get_title();
	std::cout << endl << title << endl;

	return 0;
}