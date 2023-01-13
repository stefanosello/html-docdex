// -*- lsst-c++ -*-
/*
 * @file html_handler.hpp
 * @author Stefano Sello
 * @date August 2020
 *
 * HtmlHandler namespace header file.
 * Documentation for each method can be found in html_handler.cpp.
 */

#ifndef HTML_HANDLER_H
#define HTML_HANDLER_H

#include <string>
#include <regex>

using namespace std;

namespace HtmlHandler
{
	string strip_tags(string html_string);
	string get_escaped_html_content(string str);
	string get_tag_content(string html_string, string searched_tag);
}

#endif
