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

int main(int argc, char **argv)
{
	sqlite3 *db_connection;
	HtmlDocument *doc;
	int error_status;

	DbHandler *db_handler = new DbHandler("docdex.db");
	db_handler->init_db();

	for (int i = 1; i < argc; i++)
	{
		if (i + 1 != argc)
		{
			if (strcmp(argv[i], "-i") == 0)
			{
				string document_url(argv[i + 1]);
				doc = new HtmlDocument(document_url);
				db_handler->insert_html_document_data(doc);
				return 0;
			}
			if (strcmp(argv[i], "-s") == 0)
			{
				string search_term(argv[i + 1]);
				db_handler->get_data(search_term);
				return 0;
			}
		}
	}

	return 0;
}
