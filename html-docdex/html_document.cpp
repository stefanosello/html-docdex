// -*- lsst-c++ -*-
/*
* @file html_document.cpp
* @author Stefano Sello
* @date August 2020
*
* html_document.cpp is the implementation file for the class HtmlDocument.
* It represents an html document that is going to be indexed and has all methods needed
* to download and parse the document.
*/

#include "html_document.hpp"
#include "html_handler.hpp"
#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <algorithm>

using namespace std;

/**
* Callback for the curl download method.
* Basically, writes the downloaded content into a given file
* in order to make it available for later use.
* 
* @param ptr The output file pointer
* @param size The size of the html data downloaded
* @param nmemb
* @param stream The input stream of the downloaded document
* @returns the fwrite operation result
*/
static size_t get_html(void *ptr, size_t size, size_t nmemb, void *stream) {
  int written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

/**
* HtmlDocument class constructor.
* Takes the url of the document as a string and uses cUrl library
* to save its content as a string into content attribute.
* In order to do so, creates a temoporary file where to save the downloaded html page.
* 
* @param source_url The url of the document to index
*/
HtmlDocument::HtmlDocument(string source_url) {
	string html_filename = "tmp/tmp_html.html";
	CURL *curl = curl_easy_init();
	CURLcode curl_result;

	if(curl) {
  	FILE *tmp_html;
		tmp_html = fopen(html_filename.c_str(),"w");

		this->source_url = source_url;

		curl_easy_setopt(curl, CURLOPT_URL, source_url.c_str());
	  curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
	  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,1);
	  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_html);
	  curl_easy_setopt(curl, CURLOPT_WRITEDATA, tmp_html);

		curl_result = curl_easy_perform(curl);
		if (curl_result == CURLE_OK) {
			this->tmp_filename = html_filename;
			fclose(tmp_html);
		}

		curl_easy_cleanup(curl);
	}

	if (curl_result == CURLE_OK) {
		ifstream file(this->tmp_filename);
		if(file) {
      ostringstream string_stream;
      string_stream << file.rdbuf(); // reading data
      this->content = string_stream.str();
      std::transform(this->content.begin(), this->content.end(), this->content.begin(), ::tolower);
      remove(this->tmp_filename.c_str());
   	} else {
   		perror("No temporary html file found");
   	}
	} else {
		perror("cURL request failed");
	}

}

/**
* Getter for content attribute
* 
* @returns the content attribute as a string
*/
string HtmlDocument::get_content() {
	return this->content;
}


/**
* Getter for the source url
* 
* @returns the source url of the document
*/
string HtmlDocument::get_url() {
  return this->source_url;
}


