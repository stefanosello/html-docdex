// -*- lsst-c++ -*-
/*
 * @file stemmer.cpp
 * @author Stefano Sello
 * @date August 2020
 *
 * This namespace contains only the method needed to get the stem of a word.
 * The algorithm is the classic Porter algorithm (https://tartarus.org/martin/PorterStemmer/def.txt).
 * The code is freely adapted by the node.js version made by Jed Parson (https://github.com/jedp/porter-stemmer).
 */

#include <map>
#include <regex>
#include <string>
#include <algorithm>
#include "stemmer.hpp"
using namespace std;

/**
 * Stemmer algorithm.
 *
 * @param word
 * @returns stem of the input word (in english language)
 */
string Stemmer::stem(string word)
{

	/* variables declaration - algorithm parameters */
	map<string, string> step2list;
	map<string, string> step3list;

	const string c = "[^aeiou]";
	const string v = "[aeiouy]";
	const string V = v + "[aeiou]*";
	const string C = c + "[^aeiouy]*";

	const string s_v = "^(" + C + ")?" + v;
	const string mgr0 = "^(" + C + ")?" + V + C;
	const string mgr1 = "^(" + C + ")?" + V + C + V + C;
	const string meq1 = "^(" + C + ")?" + V + C + "(" + V + ")?$";

	step2list["eli"] = "e";
	step2list["alli"] = "al";
	step2list["bli"] = "ble";
	step2list["izer"] = "ize";
	step2list["logi"] = "log";
	step2list["ator"] = "ate";
	step2list["alism"] = "al";
	step2list["aliti"] = "al";
	step2list["iviti"] = "ive";
	step2list["enci"] = "ence";
	step2list["anci"] = "ance";
	step2list["ation"] = "ate";
	step2list["entli"] = "ent";
	step2list["ousli"] = "ous";
	step2list["biliti"] = "ble";
	step2list["ational"] = "ate";
	step2list["tional"] = "tion";
	step2list["ization"] = "ize";
	step2list["iveness"] = "ive";
	step2list["fulness"] = "ful";
	step2list["ousness"] = "ous";

	step3list["ful"] = "";
	step3list["ness"] = "";
	step3list["ative"] = "";
	step3list["ical"] = "ic";
	step3list["iciti"] = "ic";
	step3list["alize"] = "al";
	step3list["icate"] = "ic";

	string stem;
	string suffix;
	string firstch;
	regex re;
	regex re2;
	regex re3;
	regex re4;

	smatch fp;

	std::transform(word.begin(), word.end(), word.begin(), ::tolower);

	if (word.length() <= 3)
		return word;

	// Step 1a
	re = "^(.+?)(ss|i)es$";
	re2 = "(.+?)([^s])s$";
	regex_replace(word, re, "$1$2");
	regex_replace(word, re2, "$1$2");

	// Step 1b
	re = "^(.+?)eed$";
	re2 = "^(.+?)(ed|ing)$";
	if (regex_match(word, fp, re))
	{
		re.assign(mgr0);
		if (regex_match(fp.str(1), re))
		{
			re = ".$";
			word = regex_replace(word, re, "");
		}
	}
	else if (regex_match(word, fp, re2))
	{
		stem = fp.str(1);
		re2.assign(s_v);
		if (regex_match(stem, re2))
		{
			word = stem;
			re2 = "(at|bl|iz)$";
			re3 = "([^aeiouylsz])\\1$";
			re4 = "^" + C + v + "[^aeiouwxy]$";
			if (regex_match(word, re2) || regex_match(word, re4))
				word = word + "e";
			else if (regex_match(word, re3))
			{
				re = ".$";
				regex_replace(word, re, "");
			}
		}
	}

	// Step 1c
	re = "^(.+?)y$";
	if (regex_match(word, fp, re))
	{
		stem = fp.str(1);
		re.assign(s_v);
		if (regex_match(stem, re))
			word = stem + "i";
	}

	// Step 2
	re = "^(.+?)(ational|tional|enci|anci|izer|bli|alli|entli|eli|ousli|ization|ation|ator|alism|iveness|fulness|ousness|aliti|iviti|biliti|logi)$";
	if (regex_match(word, fp, re))
	{
		stem = fp.str(1);
		suffix = fp.str(2);
		re.assign(mgr0);
		if (regex_match(stem, re))
			word = stem + step2list[suffix];
	}

	// Step 3
	re = "^(.+?)(icate|ative|alize|iciti|ical|ful|ness)$";
	if (regex_match(word, fp, re))
	{
		stem = fp.str(1);
		suffix = fp.str(2);
		re.assign(mgr0);
		if (regex_match(stem, re))
			word = stem + step3list[suffix];
	}

	// Step 4
	re = "^(.+?)(al|ance|ence|er|ic|able|ible|ant|ement|ment|ent|ou|ism|ate|iti|ous|ive|ize)$";
	re2 = "^(.+?)(s|t)(ion)$";
	if (regex_match(word, fp, re))
	{
		stem = fp.str(1);
		re.assign(mgr1);
		if (regex_match(stem, re))
			word = stem;
	}
	else if (regex_match(word, fp, re2))
	{
		stem = fp.str(1) + fp.str(2);
		re2.assign(mgr1);
		if (regex_match(stem, re2))
			word = stem;
	}

	// Step 5
	re = "^(.+?)e$";
	if (regex_match(word, fp, re))
	{
		stem = fp.str(1);
		re.assign(mgr1);
		re2.assign(meq1);
		re3.assign("^" + C + v + "[^aeiouwxy]$");
		if (regex_match(stem, re) || (regex_match(stem, re2) && !(regex_match(stem, re3))))
			word = stem;
	}

	re = "ll$";
	re2.assign(mgr1);
	if (regex_match(word, re) && regex_match(word, re2))
	{
		re = ".$";
		word = regex_replace(word, re, "");
	}

	return stem.length() > 0 ? stem : word;
}
