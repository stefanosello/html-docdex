#ifndef HTML_DOCUMENT_H
#define HTML_DOCUMENT_H
#include <curl/curl.h>
#include <string>

class HtmlDocument
{
private:
	CURLcode content;
public:
	HtmlDocument(char *url);
	CURLcode get_content();
	std::string get_title();
};


#endif