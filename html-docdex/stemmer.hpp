// -*- lsst-c++ -*-
/*
* @file stemmer.hpp
* @author Stefano Sello
* @date August 2020
*
* Stemmer namespace header file.
* Documentation for each method can be found in stemmer.cpp.
*/

#ifndef STEMMER_H
#define STEMMER_H

#include <string>
using namespace std;

namespace Stemmer
{
  string stem(string word); 
}

#endif