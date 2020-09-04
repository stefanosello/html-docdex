#ifndef HTML_HANDLER_H
#define HTML_HANDLER_H

#include <string>
#include <regex>

using namespace std;

namespace HtmlHandler {
  string strip_tags(string html_string);
  string get_tag_content(string html_string, string searched_tag);
  string get_escaped_html_content(string str);
}

#endif