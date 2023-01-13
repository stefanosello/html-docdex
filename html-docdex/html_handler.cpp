// -*- lsst-c++ -*-
/*
 * @file html_handler.cpp
 * @author Stefano Sello
 * @date August 2020
 *
 * html_handler.cpp is the implementation file for the HtmlHandler namespace methods.
 * Methods contained in this namespace are usefull to maipulate Html-formatted strings.
 */

#include <string>
#include "html_handler.hpp"
using namespace std;

/**
 * Strips tags from a given html string
 * taking only their textual content.
 *
 * @param html_string The input html string
 * @returns the html input string without tags wrapping
 */
string HtmlHandler::strip_tags(string html_string)
{
	string stripped_string = "";
	int length = html_string.length();
	bool tag = false;

	for (int i = 0; i < length; i++)
	{
		switch (html_string.at(i))
		{
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

		if (!tag && html_string.at(i) != '>')
		{
			stripped_string += html_string.at(i);
		}
	}
	return stripped_string;
}

/**
 * Searches for the contents of a specific tag in a given html string
 * and puts them together into a single string.
 *
 * @param html_string The input html string
 * @returns the text found in the given tag
 */
string HtmlHandler::get_tag_content(string html_string, string searched_tag)
{
	string html;
	string tag_content = "";
	html.assign(get_escaped_html_content(html_string));
	int length = html_string.length();
	bool tag = false;

	size_t found;
	while ((found = html.find("<" + searched_tag)) != string::npos)
	{
		char ch_after_match = html.at(found + searched_tag.length() + 1);
		if (ch_after_match == '>' || ch_after_match == ' ')
		{
			size_t content_start = ch_after_match == ' ' ? (html.find(">", found) + 1) : (found + searched_tag.length() + 2);
			size_t content_end = html.find("</" + searched_tag + ">");
			tag_content += html.substr(content_start, (content_end - content_start));
			html = html.substr(content_end + searched_tag.length() + 3);
		}
		else
		{
			/* the found tag is not what we are searching for, we have to go on */
			html = html.substr(found + searched_tag.length());
		}
	}

	return tag_content;
}

/**
 * Handle an html string substituting html entities with their ascii corrispective
 *
 * @param html_string The input html string
 * @returns the parsed string
 */
string HtmlHandler::get_escaped_html_content(string str)
{
	string subs[] = {"&quot;", "&apos;", "&amp;", "&lt;", "&gt;"};
	string reps[] = {"\"", "'", "&", "<", ">"};
	size_t found = 0;
	for (int j = 0; j < 5; j++)
	{
		while ((found = str.find(subs[j])) != string::npos)
		{
			str.replace(found, subs[j].length(), reps[j]);
		}
	}
	return str;
}
