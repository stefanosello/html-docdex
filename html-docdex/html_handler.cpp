#include <string>
#include <regex>

#include "html_handler.hpp"

using namespace std;

namespace HtmlHandler {

  class MalformedHTMLException: public exception {
    virtual const char* what() const throw() {
      return "The provided html document is malformed and, thus, cannot be processed and indexed.";
    }
  } malformed_html_exception;

  string strip_tags(string html_string) {
    string stripped_string = "";
    int length = html_string.length();
    bool tag = false;

    for (int i = 0; i < length; i++) {
      switch(html_string.at(i)) {
        case '<':
          tag = true;
          break;
        case '>':
          tag = false;
          stripped_string += " ";
          break;
        default:
          break;
      }

      if (!tag && html_string.at(i) != '>') {
        stripped_string += html_string.at(i);
      }
    }
    return stripped_string;
  }

  string get_tag_content(string html_string, string searched_tag) {
    string html;
    string tag_content = "";
    html.assign(get_escaped_html_content(html_string));
    int length = html_string.length();
    bool tag = false;

    size_t found;
    while ((found = html.find("<" + searched_tag)) != string::npos) {
      char ch_after_match = html.at(found + searched_tag.length() + 1);
      if (ch_after_match == '>' || ch_after_match == ' ') {
        size_t content_start = ch_after_match == ' ' ? (html.find(">", found) + 1) : (found + searched_tag.length() + 2);
        size_t content_end = html.find("</" + searched_tag + ">");
        tag_content += html.substr(content_start, (content_end - content_start));
        html = html.substr(content_end + searched_tag.length() + 3);
      } else {
        /* the found tag is not what we are searching for, we have to go on */
        html = html.substr(found + searched_tag.length());
      }
    }

    return tag_content;
  }

  string get_escaped_html_content(string str) {
    string subs[] = {"&quot;","&apos;","&amp;","&lt;","&gt;"};
    string reps[] = {"\"","'","&","<",">"};
    size_t found = 0;
    for(int j = 0; j < 5; j++) {
      while((found = str.find(subs[j])) != string::npos) {
        str.replace(found,subs[j].length(),reps[j]);
      }
    }
    return str;
  }
}