/*
    This file is part of delphi2cpp, written by Ivan Vecerina, (C) 2003-2007.

    delphi2cpp is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    delphi2cpp is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with delphi2cpp.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <set>
#include <map>
#include <string>
#include <sstream>






typedef std::string DPath;

DPath changeFileExt(DPath const& path, std::string const& newExt);
std::string baseName(DPath const& path);







int my_stricmp(const char* p, const char* q);
inline int my_stricmp(std::string const& p, std::string const& q) { return my_stricmp(p.c_str(),q.c_str()); }


/** Simple predicate for insensitive string sorting, based on std::stricmp.
Note: stricmp is actually not a standard function, so your platform might not support it.
*/
struct LessStringNoCase
{
   bool operator()(std::string const& a, std::string const& b) const
   { return my_stricmp( a.c_str(), b.c_str() ) < 0; }

   bool operator()(char const* a, char const* b) const
   { return my_stricmp( a, b ) < 0; }
};

extern char const kBlankChars[];// = " \t\n\r";

std::string trimmed(std::string const& str, char const* sepSet=kBlankChars);
std::string rtrimmed(std::string const& str, char const* sepSet=kBlankChars);

/// Converts any type to a string by sending it to a plain ostringstream.
template<typename T>
std::string stringof(T const& val)
{
   std::ostringstream ss;
      ss << val;
   return ss.str();
}

bool replaceFirst(std::string& ioStr, std::string const& findStr, std::string const& replStr);
std::string toupper(std::string const& str);






/// Mapping of a set of strings onto another (for preset identifier conversions)
typedef std::map<char const*,char const*,LessStringNoCase> DStrPtrMap;


/** Table used to apply a default mapping to all identifier received.
Used to convert Delphi types to C++ equivalents,
or to fix the case of some key identifiers.
*/
extern DStrPtrMap  sIDFilter;  /// map used to filter identifier names received


void addIDFilterTable( char const*const* pairTable );

extern const char* const baseIDFilterTable[]; // Default conversion of standard identidiers



/// A collection of case-insensitive strings.
typedef std::set<std::string,LessStringNoCase> DStrCaselessSet;
extern DStrCaselessSet     sIDCaseSet;


char const* filterID( char const* id ); // convert a delphi source identifier




