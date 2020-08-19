#include "html_document.hpp"
#include <curl/curl.h>
#include <string>
#include <regex>
#include <iostream>

using namespace std;

HtmlDocument::HtmlDocument(char* source_url) {
	CURL *curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, source_url);
		this->content = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
}

CURLcode HtmlDocument::get_content() {
	return this->content;
}

std::string HtmlDocument::get_title() {
	regex title_regex("<title(?:.*)>(.*)</title>");
	cmatch title_matches;
	string title = "";

	regex_search("<title>ciao</title>", title_matches, title_regex);

	for (unsigned i=0; i < title_matches.size(); ++i) {
	    title+= title_matches.str(i);
	}

	return title;
}

