// -*- lsst-c++ -*-
/*
* @file html_document.hpp
* @author Stefano Sello
* @date August 2020
*
* HtmlDocument class header file.
* Documentation for each method can be found in html_document.cpp.
*/

#ifndef HTML_DOCUMENT_H
#define HTML_DOCUMENT_H

#include <curl/curl.h>
#include <string>

using namespace std;

static size_t get_html(void *ptr, size_t size, size_t nmemb, void *stream);

class HtmlDocument
{
private:
	string content;
  string source_url;
  string tmp_filename;
public:
	HtmlDocument(string source_url);
	string get_content();
	string get_tag_content(string tag);
  string get_url();
};


#endif