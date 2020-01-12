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
#include "string_utils.h"

#include <cctype>
using namespace std;





/// Returns index in path where the file extension begins (=last dot).
static DPath::size_type extPosInPath(DPath const& path)
{
   DPath::size_type n = path.find_last_of(".\\/:");
   return ( path[n]=='.' ) ? n : DPath::npos;
}

/// Returns index in @a path where the file name begins
static DPath::size_type namePosInPath(DPath const& path)
{
   DPath::size_type n = path.find_last_of("\\/:");
   return ( DPath::npos == n ) ? 0 : n+1;
}

/// Returns @a path with a different file extension
DPath changeFileExt(DPath const& path, std::string const& newExt)
{
   return DPath( path, 0, extPosInPath(path) ) + newExt;
}

/// Returns the extension-free file name extracted from @a path
std::string baseName(DPath const& path)
{
   DPath::size_type n = namePosInPath(path);
   return DPath( path, n, extPosInPath(path)-n );
}








/// Case-insensitive string comparisons (is provided by some platforms as stricmp).
int my_stricmp(const char* p, const char* q)
{
   for(;;) {
      int c = toupper( (unsigned char)*p++ );
      int d = toupper( (unsigned char)*q++ );
      if( c != d  ) return (c<d) ? -1 : +1;
      if( c=='\0' ) return 0; // => end of string: c == d == '\0'
   }
}

char const kBlankChars[] = " \t\n\r";

std::string rtrimmed( std::string const& str, char const* sepSet )
{
   std::string::size_type const last = str.find_last_not_of(sepSet);
   return ( last==std::string::npos )
               ? std::string()
               : str.substr(0, last+1);
}


std::string trimmed( std::string const& str, char const* sepSet )
{
   std::string::size_type const first = str.find_first_not_of(sepSet);
   return ( first==std::string::npos )
               ? std::string()
               : str.substr(first, str.find_last_not_of(sepSet)-first+1);
}



/// Replaces in @a ioStre the first occurence of @a findStr with @a replStr. @return true iff a substitution was made.
bool replaceFirst(std::string& ioStr, std::string const& findStr, std::string const& replStr)
{
   std::string::size_type const pos = ioStr.find(findStr);
   if( std::string::npos == pos )
      return false;

   ioStr.replace( pos, findStr.size(), replStr );
   return true;
}

/// Returns @a str as a lowercase string -- converted using std::toupper.
std::string toupper(std::string const& str)
{
   std::string ans( str.size(), '\0' );
   std::string::const_iterator src = str.begin();
   std::string::const_iterator end = str.end();
   std::string::iterator dst = ans.begin();
   for(  ; src != end ; ++src, ++dst )
      *dst = (char)std::toupper((unsigned char)(*src)); //20050624: parentheses added around (unsigned char) for portability reasons (Borland C++)
   return ans;
}







DStrPtrMap  sIDFilter;  /// map used to filter identifier names received

/// Load a static table of string pairs into sIDFilter (see baseIDFilterTable).
void addIDFilterTable( char const*const* pairTable )
{
   while( const char* src = *pairTable++ )
      sIDFilter.insert( make_pair<const char*,const char*>(src, *pairTable++) );
}



/** Table where previously encountered identifiers are stored.
    Used to convert subsequent encouters of the same identifier
    to use the same case (C++ is case-sensitive, Delphi is not).
*/
DStrCaselessSet     sIDCaseSet;


/* can be used to enforce a specific case to some identifiers ... currently not used
void addIDCaseDefs( const char* const* idTable )
{
   while( const char* id = *idTable++ )
      sIDCaseSet.insert(id);
}
*/


/// Returns @a id, or a converted indentifier if it is defined in sIDFilter.
char const* filterID( char const* id )
{
   DStrPtrMap::iterator const pos = sIDFilter.find(id);
   if( pos!=sIDFilter.end() )  return pos->second;

   // translate the identifier using or table of previous encounters
   return sIDCaseSet.insert(id).first->c_str();
}


/// Default conversion of standard identidiers
const char* const baseIDFilterTable[] =
   // conversion of common Delphi and OpenGL types
   { "integer", "int"
   , "dword", "unsigned long"
   , "longint", "long"
   , "single", "float"
   , "real",   "double"
   , "double", "double"
   , "byte", "unsigned char"
   , "word", "unsigned short"
   , "longword", "unsigned long"
   , "cardinal", "unsigned long"
   , "shortstring", "std::string"
   , "shortint", "signed char"
   , "smallint", "short"
   , "int64", "__int64"
   , "char", "char"
   , "boolean", "bool"
   // Common Pointer and Windows types
   , "PChar", "char*"
   , "PWord", "unsigned short*"
   , "PDWORD", "unsigned long*"
   , "THandle", "HANDLE"
   // some OpenGL stuff
   , "gldouble", "double"
   , "glfloat", "float"
   , "glboolean", "bool"
   , "glbyte", "unsigned char"
   // common math functions
   , "arcsin", "asin"
   , "arccos", "acos"
   , "arctan", "atan"
   , "arctan2", "atan2"
   // case-conversions
   , "true", "true"
   , "false", "false"
   , "result", "result"
   , "min", "min"
   , "max", "max"
   , "abs", "abs"
   , "sqrt", "sqrt"
   , "cos", "cos"
   , "sin", "sin"
   , "tan", "tan"
   , "atan", "atan"
   , "Assigned", "!!"
//   , "", ""
   // known identifiers of custom functions/types/...
   , "StrToFloat", "atof"

// NOTE: you may expand this list with your own identifier translations
// , "IdentifierInSource_not_case_sensitive", "replacementIdentifier"
   , "vec3f", "vec3f"
   , "vec2f", "vec2f"
   , "vec3i", "vec3i"
   , "init3f", "vec3f"
   , "init2f", "vec2f"
   , "init3i", "vec3i"

// list ends with a NULL string pointer:
   , 00
   };



