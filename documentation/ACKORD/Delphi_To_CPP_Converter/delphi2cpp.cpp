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
/*
@file   Delphi2cpp.cpp
@brief  A delphi-to-c++ conversion utility.

@author    Ivan Vecerina, (C) 2003-2007

@par    CHANGE HISTORY

20031013 (some suggestions of Jorgen Bodde - others pending)
 - added additional identifier mappings (i.e. PChar)
 - restored proper handling of characters given as ASCII value using #dd syntax
20031126 (some problems reported by Mark Philip Erikson, while trying to translate sources of DevCPP)
 - fixed parsing error with pascal strings of a specified size (e.g.  var a: string[255] )
 - appendPlusOne: directly increment all decimal ordinal values (instead of appending +1)
 - added missing support for the 'published' member access specification in delphi classes (simply -> public)
 - fixed failures on DEFAULT property, ASM functions, MESSAGE and STDCALL function labels.
 also a request from Grzegorz Majcher [GM]:
 - allowed 'forward' keyword to be applied to a routine declaration in a unit's interface
20040410 (fixed some newly reported serous bugs)
 - Fixed incorrect lexical pattern for processing (*  *) comments [bug reported by Matt Minnis]
 - Support the definition of derived classes with no body, e.g. MyExcept=class(Exception); [bug reported by Matt Minnis]
 - Support for 'program' files (not just 'unit's)  [request from Marcos(ssitecnologia) ]
20050624:
 - a bug fix, and a fix to an incompatibility with Borland C++ [reported by Grzegorz Majcher]
20060701:
 - fixes contributed by Guru Kathiresan: parsing THREADVAR, OUT, REINTRODUCE and FAR keywords, pass on ASM blocks as comment.
 - basic handling of property members, based on submission by Guru.
 - added handling of "on x: ExceptionType" clauses within an EXCEPT block.
 - parse case where the size of an array is given using an enumarated type identifier.
 - handling of RAISE statements
20050806:
 - Added missing support for multi-dimensional arrays - issue reported by A. Fokin
20050812:
 - Added missing support for use of labels & gotos (label declarations, label use, goto statement) - issue reported by S. Korotky
20060906:
 - Translate "not assigned" to "!" instead of "! !!" ; Support for SAFECALL ; 
20060917:
 - support for writing CLASS PROCEDURE/FUNCTION in the IMPLEMENTATION (redundant but legal repeat - contributed by A. Tompe
 - support for dot-separated identifiers in USES section (a Delphi.NET feature, pointed out by A. Tompe)
20070227 - requests from C. Viavattene:
 - avoid termination of translation if an empty property declaration is encountered ("property OnMouseDown;")
20070407 - requests from S. Vilcans:
 - translate CLASS or INTERFACE to "class" with "public:" inserted if needed, rather than as "struct"
 - improved handling of constructors/destructors: replace "Create"/"Destroy" with actual class name

@par   ENHANCEMENT REQUESTS
These are change requests that were seen as not critically needed by enough people,
or that would take me more time than I can dedicate / go beyond the intended scope of this translator.
 - Add parameter list when omitted in the implementation of a function/procedure (if declaration was encountered) [Ihor Bobak]
 - Add the translation of more native types from windows.pas (e.g. THandle -> HANDLE) [IB]
 - Windows API functions: translate var parameters to pointer instead of references when adequate [IB]
 - Read the list of default-mapped identifiers in a configuration/text file, to allow easy customization. [JB]
 - Keep trace of type identifiers that are of a pointer type, vs. of value/struct type [JB]
 - Keep info about types, so that, for example, Delphi's TObject-derived parameters [JB]
   can be passed by pointer instead of by value in the translated C++ code.
   Would also allow to translate '.' into '->' when appropriate.
 - Support for translating 'case' within records into C unions [GM]
 - Support for using anonymous records defined on-the-fly: the following construct is currently rejected by delphi2cpp:
     TempRulesWord:  array of packed record  Ending: Word; Feature: Word;  end;
 - Support for uses of sets of anonymous enumerations, for example:   type  TNumber = set of (cFirst, cSecond); 
 - translate {$IFDEF...} and other directives into corresponding preprocessor macros: #ifdef...
*/
#include "delphi_yy.h"    ///< handles the reading of the pascal/delphi input file
#include "string_utils.h" ///< string manipulation utilities (path/identifier)

#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>


/*
############
The following code can not be converted:

TempRulesWord: array of packed record
           Ending: Word;
           Feature: Word;
        end;

In the following method...

function TMYobject.Test(Index: Integer): Byte;
begin
  if (not Assigned(Self))  then
    begin
      Exit;
    end;
end;

we get the if as 'if ( ! !!Self)' in C++ - is it ok? Why not simply '!Self'?

##################################
*/

using namespace std;


//  INTERNAL UTILITIES AND GLOBALS
   ofstream hdst; ///< output to the written .h file
   ofstream cdst; ///< output to the written .cpp file

   /** Current output file (usually points to hdst or cdst).
       By default, indentation, spaces and comments encountered during source file
       reading are automatically forwarded to pdst -- to try to preserve code layout.
       Can be set to NULL to disable this automated output. */
   ostream* pdst;

   bool     sTokPendingIsValid = false; ///< false if sTokPending must be read
   EToken   sTokPending = eEOF;  ///< Next token (if have been peeked/pushed back)

   /// When parsing declarations, indicates the kind of scope we are in
   enum EDeclScope { eUnitInterf, eUnitImpl, eFuncBody, eFuncParam, eInRecord, eInClass, eInInterf };
   bool  sInFunc; ///< True when parsing a function body (vs. false for a Pascal PROCEDURE).



#define QMARK "/*?*/" ///< written to output when we know something is not properly translated
#define TMARK "<###>" ///< used as a temporary token when constructing composite type definitions



/**  Temporarily redirects the default output (see pdst global variable above).
     Constructor sets pdst to the specified target.
     Destructor restores the previous value of pdst.
*/
class pdstRedirect
{
 public:
    pdstRedirect(ostream& dst)  : prev_(pdst) { pdst = &dst; }
    pdstRedirect(ostream* dst)  : prev_(pdst) { pdst = dst; }
   ~pdstRedirect()             { pdst = prev_; }
   ostream*  prev() const      { return prev_; }
 private:
   ostream* const prev_;
   pdstRedirect(pdstRedirect const&);            ///< Disabled copy constructor
   pdstRedirect& operator=(pdstRedirect const&); ///< Disabled default assignment
};



/** Changes a value and automatically restore it at scope exit.
The previous value of a variable is restored upon destruction of a ScopedSet instance.
*/
template<class T>
class ScopedSet
{
 public:
   ScopedSet(T& var, T const& tempVal) : var_(var), oldVal_(var) { var = tempVal; }
   ~ScopedSet()                        { var_ = oldVal_; }

 private:
   T& var_;     ///< variable whose value will be restored
   T  oldVal_;  ///< value to be restored upon destruction
   ScopedSet(ScopedSet const&);            ///< Disabled copy constructor
   ScopedSet& operator=(ScopedSet const&); ///< Disabled default assignment
};


/// Abort the translation of the file with 'msg' as an error description
void fail(char const* msg)
{
   throw std::runtime_error(msg);
}
/// Handling of failed assertions ( calls fail() ).
void assertFail(char const* msg, const char* file, int line)
{
   stringstream strm;
   strm << "ASSERT failure: "<<msg<<" -- on line "<<line<<" of "<<file;
   fail(strm.str().c_str());
}
/// Simple implementation of an assertion-handling macro...
#define ASSERT( test ) if(test);else assertFail( #test , __FILE__, __LINE__ );


/// Returns (or 'ungets') the last input token that was read using nextToken().
void putBackToken()
{
   ASSERT( !sTokPendingIsValid );
   sTokPendingIsValid = true;
}

/** Returns the next token from the input file.
Automatically copies whitespace and comments from the input file
to the current destination file -- if one is specified by the pdst global.
One last read token can be put back using putBackToken() -- the next
call to EToken will then read the same token again.
@return A token identifier.
@note   The textual representation of the read token can
        be accessed using the delphi_yytext global variable.
*/
EToken nextToken()
{
   if( sTokPendingIsValid )
      { sTokPendingIsValid = false; return sTokPending; }

   for(;;)
   {
      switch( sTokPending = delphi_yylex() ) {
   case eBlanks:
   case eCommentLine:  // copy blanks and line comments to destination
         if(!!pdst) pdst->write(delphi_yytext, delphi_yyleng);
         continue;
   case ePRE_IFDEF:
   case ePRE_IFNDEF:
   case ePRE_ELSE:
   case ePRE_ENDIF:
         if(!!pdst) *pdst << QMARK;
   case eCommentBlock: // transfer contents of comment blocks
         if(!!pdst) {      //TODO: should check for */ and replace it...
            int skip = (delphi_yytext[0]=='{') ? 1 : 2;
            pdst->write("/*",2).write(delphi_yytext+skip,delphi_yyleng-2*skip).write("*/",2);
         }
         continue;
   default:
      return sTokPending;
      }
   }
}

/** Require a token of type @a tok, and return its textual representation.
fail() is called if the next token does not match @a tok.
*/
char const* requireToken(EToken tok)
{
   if( nextToken() != tok )
      fail( ("Unexpected token: "+std::string(delphi_yytext)).c_str() );
   return delphi_yytext;
}

/** Attempts to retrieve a token of type @a tok from the input.
@return true if the token was found, false if the following token is not a match and was not read.
*/
bool tryGetToken(EToken tok)
{
   if( nextToken() == tok )
      return true;
   putBackToken();
   return false;
}

/// Returns the type of the next token, without actually reading it.
EToken peekToken()
{
   EToken ans = nextToken();
   putBackToken();
   return ans;
}

/// Returns true if the next token is an identifier (not an operator or keyword).
bool peekNextIsIdentifier()
{
   EToken const tok = nextToken();
   putBackToken();
   return ( tok == eID  ||  (tok>=eFirstSemiKeyword && tok<=eLastSemiKeyword) );
}

/** Require an identifier as the next token and return it as a string.
The returned identifier string is filtered (see filterID() ).
fail() is called if the next token is not an identifier.
*/
char const* requireIdentifier()
{
   EToken const tok = nextToken();
   if( tok != eID  &&  (tok<eFirstSemiKeyword || tok>eLastSemiKeyword) )
      fail( "Identifier was expected" );

   return filterID(delphi_yytext);
}

/** Require a simple type identifier as the next token and return it as a string.
This function is somewhat more "flexible" than requireIdentifier in that it also accepts a keyword identifying a primitive type (e.g. eSTRING).
fail() is called if the next token is not an identifier.
*/
char const* requireTypeIdentifier()
{
   if( peekToken()==eSTRING ) { return "std::string"; } //TBD: more cases may need to be handled here:  string[n] , more?
   return requireIdentifier();
}

/// Stores a copy of @a s in @a *pOptional if @a pOptional is not Null, and returns @a s.
string assignOptionalAndReturn( std::string const& s, std::string* pOptional )
{ if(!!pOptional) *pOptional=s; return s; }

/// Gets next identifier, absorbs any scoping (using ".") and converts it to "::".
string requireScopedIdentifier(std::string* pOutLastIdentifier=0)
{
   string id = assignOptionalAndReturn( requireIdentifier(), pOutLastIdentifier);
   while( tryGetToken(eDot) )
      id.append("::"), id.append( assignOptionalAndReturn( requireIdentifier(), pOutLastIdentifier) );
   return id;
}


/** Appends a +1 to the provided expression, but tries to remove a redundant trailing -1 instead if possible.
Such a modification is commonly needed in for loops and array bounds,
because they are handled differently in Pascal and C.
*/
string appendPlusOne(string const& str)
{
   string::size_type const sz = str.size();
   string s = trimmed(str);
   long l; char* end;
   // just try to avoid a classic and ugly "Count-1+1" output.
   if     (  sz>2 && s.substr(sz-2)=="-1"  ) s.erase(sz-2);
   else if(  sz>3 && s.substr(sz-3)=="- 1" ) s.erase(sz-3);
   else if(  (l = strtol( s.c_str(), &end, 10 )), ! *end )  //ivec20031126: directly increment all decimal ordinal values
   {  // successfully interpreted as an ordinal value => increment
      s = stringof(l+1);
   }
   else s += "+1";
   return rtrimmed(s);
}


//-----------------------------------------------------------------------------------


/// Simple description of the type of an expression
enum EType { tyUndef, tyInt, tyFloat, tyString, tyChar, tyClass, tyRecord };

/** Translates the following expression from the source file.
Reads and translates an expression (part of a pascal source statement or declaration).
Attempts to guess the type of the expression and return it in @a outHint -- but this is not functioning well.
@param outHint  On output, receives a guess at the type of the expression (DOES NOT WORK WELL)
@param isInType If true, prevents the handling of some operators (e.g. = )
                stops the processing of the input (useful in some declarations...).
*/
string readExpr( EType& outHint, bool isInType=false )
{
   outHint = tyUndef;
   ostringstream dst(ios_base::out|ios_base::binary);
   peekToken();
   pdstRedirect temp(dst);

   for(;;) {
      EToken const tok = nextToken();
      switch( tok ) {
         case eCharString: {
               dst.put('"');
               for( int i = 1 ; i != delphi_yyleng-1 ; ++i )
                  switch( char c = delphi_yytext[i] ) {
                  case '\'':  ++i; //fallthru after skipping doubled inner single quote
                  case '\"':
                  case '\\':  dst.put('\\'); //fallthru to put the actual char
                  default:    dst.put(c);
                  }
               dst.put('"');
            } break;
         case eNumInt: {
               char const* s = delphi_yytext;
               if( s[0]=='$' ) { dst<<"0x"; ++s; } // special handling if hex number
               dst << s;
            } break;
         case eCharCoded:  dst << "(char)";    break; //NB: discard, or /*char*/ would be ok too
         case eTRUE:       //dst << "true";       break;
         case eFALSE:      //dst << "false";      break;
         case eRESULT:     //dst << "result";     break;
         case eID:         putBackToken(); dst << requireIdentifier(); break;
         case eEqual:      if(isInType) goto put_back_and_exit;
                           dst << "==";         break;
         case eUnequal:    dst << "!=";         break;
         case eAND:        dst << "&&";         break;
         case eOR:         dst << "||";         break;
         case eNOT:        if( peekToken()==eASSIGNED ) nextToken(); //ivec20060906: "not assigned" -> ! instead of !!!
                           dst << "!";          break;
         case eAt:         dst << "&";          break;
         case eDIV:        dst << "/*div*//";   break;
         case eDiv:        dst << "*1.0/";      break;
         case eInto:       dst << "->";         break;
         case eCirc:       dst << QMARK "^";    break;
         case eSHL:        dst << "<<";         break;
         case eSHR:        dst << ">>";         break;
         case eMOD:        dst << "%";          break;
         case eNIL:        dst << "00";         break;
         case eASSIGNED:{  if( !tryGetToken(eParIn) ) goto dflt;
                           string const& svar = readExpr(outHint);
                           dst << "!! " << svar << " ";
                           requireToken(eParOut);
                           break;
                        }
         case eINC:
         case eDEC:     {  if( !tryGetToken(eParIn) ) goto dflt;
                           string const& svar = readExpr(outHint);
                           if( tryGetToken(eParOut) ) {
                              dst << (tok==eINC ? "++":"--" ) << svar;
                              break;
                           }
                           requireToken(eComma);
                           string const& sdif = readExpr(outHint);
                           dst << svar << (tok==eINC ? " += ":" -= ") << sdif;
                           requireToken(eParOut);
                           break;
                        }

dflt:    default:          dst.write(delphi_yytext,delphi_yyleng);
                           break;

         case eParIn:      dst << "( " << readExpr(outHint);
                           while( tryGetToken(eComma) )
                              dst << ", " << readExpr(outHint);
                           dst << " )";
                           requireToken(eParOut);
                           break;

         case eBrackIn:    dst << "[ " << readExpr(outHint);
                           for(;;) {
                              if( tryGetToken(eComma) )
                                 dst << ", " << readExpr(outHint);
                              else if( tryGetToken(eRange) )
                                 dst << " .. " << readExpr(outHint);
                              else break;
                           }
                           dst << " ]";
                           requireToken(eBrackOut);
                           break;

         case eColon:      case eSemic:   case eAssign:    case  eComma:
         case eBrackOut:   case eParOut:  case eRange:
         case eELSE:       case eTHEN:    case eUNTIL:
         case eEND:        case eEXCEPT:  case eFINALLY:
         case eTO:         case eDOWNTO:  case eDO:        case eOF:
put_back_and_exit:         putBackToken();
                           return trimmed(dst.str());
      }
   }
}


/// Read the signature/type of a function (parameters and return type).
string readFuncSig(bool isFunc, bool isCtrOrDtr=false, string* retTypeSig=0);

/** Translate a type signature into a declaration.
The occurrence of string TMARK within @a typeDecl is replaced with @a id.
*/
string makeTypeDecl(string const& typeDecl, string const& id)
{
   string ans = typeDecl;
   bool subst = replaceFirst(ans,TMARK,id);
   ASSERT( subst );
   return ans;
}

/** Read a type signature.
The returned string is a type declaration, where the TMARK string is
inserted instead of a specific identifier (to allow later substitution).
*/
string readTypeDecl()
{
   string id = TMARK;
   pdstRedirect temp(00);
   EType th;

   bool isFunc = tryGetToken(eFUNCTION);
   if( isFunc || tryGetToken(ePROCEDURE) ) {
      id = readFuncSig(isFunc);
      // convert to a function-pointer type
      id = makeTypeDecl( id, "(*" TMARK ")" );
      if( tryGetToken(eOF) ) { // proc/func of object declaration -- not properly translated
         //NB: translation to something like boost::function would make sense...
         id = id + " /* of class: " + requireScopedIdentifier() + " */" QMARK;
      }
      return id;
   }

   while( tryGetToken(eCirc) ) // ^ => pointer decl
      id = "*" + id;

   while( tryGetToken(ePACKED)/*skip*/, tryGetToken(eARRAY) )
   {
      if( tryGetToken(eBrackIn) ) {
         do { //ivec20060806: support for multi-dimensional arrays
            string slo = readExpr(th);
            if( ! tryGetToken(eRange) ) {  //ivec20060701 parse cases where range is defined by an enum type
               id = id + "[" QMARK " range of " + slo + " ]";
            }
            else {
               string shi = readExpr(th);//requireToken(eNumInt);
               char* end;
               long lo = strtol( slo.c_str(), &end, 10 );
               bool oklo = ! *end;
               long hi = strtol( shi.c_str(), &end, 10 );
               bool okhi = ! *end;
               if( oklo && lo==0 ) {
                  id = id + "[" + ( okhi ? stringof(hi-lo+1) : appendPlusOne(shi) ) + "]";
               }
               else {
                  id = id + "[ " QMARK "/*" + slo + ".." + shi + "*/ (" + shi + ")-(" + slo + ")+1 ]";
               }
            }
         } while( tryGetToken(eComma) ); //ivec20060806 comma is used to separate dimensions of a multi-dimensional array
         requireToken(eBrackOut);
      }
      else {
         id = id + "[ " QMARK " ]";
      }
      requireToken(eOF);
   }

   if( tryGetToken(eSTRING) ) {
      if( tryGetToken(eBrackIn) ) { //ivec20031126: support for fixed-size strings
         string slen = readExpr(th);
         requireToken(eBrackOut);
         id = "char" QMARK "/*string*/ "+id+"["+appendPlusOne(slen)+"]";
      }
      else
         id = "std::string " + id;
   }    /* NOT SUPPORTED HERE: it is possible to declared an unnamed record on-the-fly in Delphi... */
   else if( tryGetToken(eParIn) )
   {
      id = "enum "+id+" { "+requireIdentifier();
      while( tryGetToken(eComma) ) {
         id = id + ", " + requireIdentifier();
      }
      requireToken(eParOut);
      id = id+" }";
   }
   else if( tryGetToken(eSET) )
   {
      requireToken(eOF);
      string const ty = requireScopedIdentifier();
      id = "unsigned long" QMARK "/*set of: " + ty + " */ "+id;
   }
   else {
      EType th;
      string const ty_or_expr = readExpr(th,true);
      if(!tryGetToken(eRange))
         id = ty_or_expr + " " + id;
      else {
         string const expr2 = readExpr(th);
         id = "int" QMARK "/*range: " + ty_or_expr + " .. " + expr2 + " */ "+id;
      }
   }

   return id;
}


// Translate a sequence of statements
void transStatements();


/** Translate a single statement.
The statement may be compound: begin..end, try..catch, repeat..until, case..., if... etc.
@return true iff the statement is compound (ends with a }) ==> no semicolon should follow.
*/
void transStatement()
{
   ASSERT( !! pdst );
   ostream& dst = *pdst;
   EType th;
loop:
   EToken const tok = nextToken();
   switch( tok ) {
      default:
         putBackToken();
         dst << readExpr(th);
         if( tryGetToken(eAssign) ) {
            dst << " =";
            dst << readExpr(th);
         }
         if( tryGetToken(eColon) ) { // label
            dst << ":";
            goto loop; // read the statement that follows the label (mandatory IIRC)
         }
         break;

      case eRAISE:      dst << "throw"; if(peekToken()!=eSemic) dst<<readExpr(th); break; //ivec20060701
      case eBREAK:      dst << "break";      break;
      case eCONTINUE:   dst << "continue";   break;
      case eEXIT:       dst << "return";   if (sInFunc) dst << " result";  break;
      case eINHERITED:  dst << "inherited::";
                        if( peekToken()==eSemic ) { dst << QMARK "PROC_NAME"; break; }
                        goto loop; // read the oncoming routine call itself.

      case eGOTO:
         dst << "goto";
         dst << requireIdentifier();
         break;

      case eBEGIN:
         dst << "{";
         transStatements();
         requireToken(eEND);
         dst << "}";
         return;//no semicolon

      case eIF:
         dst << "if( ";
         dst << readExpr(th);
         requireToken(eTHEN);
         dst << " )";
         transStatement();
         if( tryGetToken(eELSE) ) {
            dst << "else";
            transStatement();
         }
         return;//no (extra) semicolon, one already sent by transStatement() call

      case eWHILE:
         dst << "while( ";
         dst << readExpr(th);
         requireToken(eDO);
         dst << " )";
         transStatement();
         return;//no (extra) semicolon, one already sent by transStatement() call

      case eWITH:
         dst << QMARK "/* WITH ";
         for(;;) {  //added 2004/11/29 - can have multiple entries <-- error report by B.Parent
            dst << readExpr(th);
            if( ! tryGetToken(eComma) ) break;
            dst << ',';
         }
         requireToken(eDO);
         dst << " */";
         transStatement();
         return;//no (extra) semicolon, one already sent by transStatement() call

      case eREPEAT:
         dst << "do {";
         transStatements();
         requireToken(eUNTIL);
         dst << "} while(!( ";
         dst << readExpr(th);
         dst << " ))";
         break;

      case eCASE:
         dst << "switch( " << readExpr(th) << " ) {";
         requireToken(eOF);
         for(;;) {
            if( tryGetToken(eELSE) ) {
               dst << "default:";
               transStatements();
               requireToken(eEND);
               break;
            }
            if( tryGetToken(eEND) )
               break;
            do {
               dst << "case " << readExpr(th);
               if( tryGetToken(eRange) )
                  dst << QMARK << ".." << readExpr(th);
               dst << ": ";
            } while( tryGetToken(eComma) );
            requireToken(eColon);
            transStatement();
            if( peekToken() != eELSE ) requireToken(eSemic);
         }
         dst << "}";
         return;//no semicolon

      case eFOR: {
         string si, sb, se;
         bool down; // downto (decrementing) loop instead of an incrementing one.
         {  pdstRedirect temp(00);
            si = requireIdentifier();
            requireToken(eAssign);
            sb = readExpr(th);
            down = tryGetToken(eDOWNTO);
            if( !down ) requireToken(eTO);
            se = readExpr(th);
            requireToken(eDO);
            // correct end value by +1 or -1 (depending on loop direction).
            if( ! down ) se = appendPlusOne(se);
         }
         dst << "{ long " << si << "_end = " << se << " ; for( "
             << si << " = " << sb << " ; " << si << (down?" >= ":" < ") << si << "_end ; "
             << (down?"--":"++") << si << " )";
         transStatement();
         dst << "}";
         return;//no semicolon
      }


      case eTRY:
         dst << "try {";
         transStatements();
         if( tryGetToken(eFINALLY) ) {
            dst << "}" QMARK "/*FINALLY*/ catch(...) {";
         }
         else {
            requireToken(eEXCEPT);
            if( peekToken()==eON ) { //ivec20060701 added support for ON... clauses within exception handling block
               dst << "}\n";
               while( tryGetToken(eON) ) { //--> either  on {type} do statement;   or  on {var}:{type} do statement;
                  string name = "";
                  string type = requireTypeIdentifier();
                  if( tryGetToken(eColon) ) { name = type; type = requireTypeIdentifier(); }
                  requireToken(eDO);
                  dst << "catch( "<<type<<" const& "<<name<<" )";
                  bool const isBlock = peekToken()==eBEGIN;
                  if( ! isBlock ) dst << " {";
                  transStatement();
                  if( ! isBlock ) dst << "}";
                  tryGetToken(eSemic);
               }
               if( ! tryGetToken(eELSE) ) { //bugfix20060906: was peekToken()!=eELSE --> left the 'else' in place and failed
                  requireToken(eEND);
                  return;
               }
               dst << "catch(...) {";
            }
            else dst << "} catch(...) {";
         }
         transStatements();
         dst << "}";
         requireToken(eEND);
         return;//no semicolon

   }

   dst << ";";
}


/** Translate a sequence of statements.
Repeatedly calls transStatement() until a block end is encountered.
*/
void transStatements()
{
   bool isSep = true;

   ASSERT( !! pdst );
   EToken tmpTok;
   for(;;) {
      tmpTok = peekToken();
      switch(tmpTok) {
      case eEND:           case eUNTIL:
      case eFINALLY:       case eEXCEPT:
      case eFINALIZATION:
         return;
     case eASM: // derived from contribution by Guru Kathiresan 200606
         nextToken();//skip eASM
         *pdst << "{"QMARK"/* ASM";
         while( nextToken() != eEND ) *pdst << delphi_yytext;
         *pdst << "ASM END */}";
         requireToken(eSemic);
         continue;
      default:
         if( !isSep )
            fail("Expected a semicolon (;).");
         transStatement();
         isSep = tryGetToken(eSemic);
      }
   }
}


//-----------------------------------------------------------------------------------

void transFunc(EDeclScope scope, bool isFunc, std::string const& className=std::string() );
void transDecls(EDeclScope scope);

/// Translate a uses section into #include instructions (dumb path translation).
void transUsesSection(EDeclScope scope)
{
   if( scope!=eUnitInterf && scope!=eUnitImpl )
      fail( "Uses section is only allowed within interface or implementation" );

   do {
      std::string unit = requireIdentifier();
      while( tryGetToken(eDot) ) { //ivec20060917: in .net, unit can be comma-separated paths --> support added here
         unit.push_back('/');
         unit.append(requireIdentifier());
      }
      *pdst << "\n#include \""<<unit<<".h\"";
   } while ( tryGetToken(eComma) );

   requireToken(eSemic);
}

/// Translate a label section into a comment block listing the labels.
void transLabelSection(EDeclScope scope)
{
   if( scope!=eFuncBody )
      fail( "Label section is only allowed within a function/procedure definition" );

   *pdst << "/* Labels:";
   do {
      char const* labelId = requireIdentifier();
      *pdst << labelId;
   } while ( tryGetToken(eComma) );

   requireToken(eSemic);
   *pdst << "  */";
}


/// Translate a Pascal/Delphi CONST declaration section.
void transConstSection(EDeclScope /*scope*/)
{
   while( peekNextIsIdentifier() )
   {
      string decl, val;
      {
         pdstRedirect temp(00);

         string const id = string("const ")+requireIdentifier();
         if( tryGetToken(eColon) )
            decl = readTypeDecl();
         else
            decl.erase();

         requireToken(eEqual);
         EType et;
         val = readExpr(et); //BUG/TODO: support init-value such as:  someConstOrVar: TGUID = (D1: $4657278B; D2: $411B:.....

         if( decl.empty() )
         {
            switch(et) {   // try to guess the type of the non-typed constant
               case tyString:    decl = "const char* ";
               case tyInt:       decl = "long ";
               case tyFloat:     decl = "float ";
               case tyChar:      decl = "char ";
               default:          decl = "double" QMARK " ";
            }
            decl = decl + id;
         }
         else
            decl = makeTypeDecl(decl, id);
      }
      *pdst << "static " << decl << " = " << val;

      requireToken(eSemic);
      *pdst << ";";
   }
}



void transVarSection(EDeclScope const scope, std::string const& className=std::string() );



/** Translate a property member into a pair of get and set functions. Based on a contribution by Guru Kathiresan 200606.
General syntax:
 property {propertyName}[ \[ indexParams... \] ] : type  [ index {intConstant} ]
          [ read  {fieldOrMethod} ]
          [ write {fieldOrMethod} ]
          [ stored trueORfalse ]
          [ default {value}  |   nodefault ]
          ; [default;]
NB 20070227:   property OnMouseDown;   (no type spec) is reported as a valid declaration (new?) -> added basic support
*/
void transProperty()
{
   ostream& dst = *pdst;
   string const propId = requireIdentifier();
   dst << QMARK << "/*PROPERTY " << propId;
   if( tryGetToken(eBrackIn) ) {
      dst << "[";
      transVarSection(eFuncParam);
      requireToken(eBrackOut);
      dst << "]";
   }
   string const propType = tryGetToken(eColon) ? requireTypeIdentifier() : ("int"QMARK);
   EType dummy;
   string const callIndex = tryGetToken(eINDEX) ? readExpr(dummy,true) : string();
   std::string read_value, write_value;
   EToken tok;
   while( (tok=nextToken()) != eSemic ) {
      switch(tok) {
      case eREAD:   read_value = requireIdentifier(); break;
      case eWRITE:  write_value = requireIdentifier(); break;
      case eSTORED:    dst<<" STORED("<<readExpr(dummy,true)<<")"; break;
      case eDEFAULT:   dst<<" DEFAULT("<<readExpr(dummy,true)<<")"; break;
      case eNODEFAULT: dst<<" NODEFAULT"; break;
      }
   }
   if( tryGetToken(eDEFAULT) ) { dst << " DEFAULT => operator[]() "; requireToken(eSemic); }
   dst << " */\n";

   if( ! read_value.empty() )
   {
      dst<<propType<<" get_"<<propId<<"()     { return ";
      if( callIndex.empty() ) dst<<read_value;
      else                    dst<<read_value<<'('<<callIndex<<')';
      dst<<"; }\n";
   }
   if( ! write_value.empty() )
   {
      dst<<"void set_"<<propId<<"( "<<propType<<" v ) { ";
      if( callIndex.empty() ) dst<<write_value<<" = v";
      else                    dst<<write_value<<"( "<<callIndex<<", v );";
      dst<<"; }\n";
   }
}


/// Translate a Pascal VAR declaration section.
void transVarSection(EDeclScope const scope, std::string const& className )
{
   ostream& dst = *pdst;
   string id, decl;
   vector<string> ids;
   for(;;) {
      if( scope==eInClass || scope==eInInterf ) {
         EToken tok = nextToken();
         switch( tok )
         {
         case ePUBLISHED:  dst << "\npublic/*published*/:";  continue; //ivec20031126: support "published"
         case ePUBLIC:     dst << "\npublic:";     continue;
         case ePROTECTED:  dst << "\nprotected:";  continue;
         case ePRIVATE:    dst << "\nprivate:";    continue;
         case eCLASS:      dst << "\nstatic"; continue; //e.g. CLASS FUNCTION...
         // within a class, the following do NOT end a variable section (they may, actually, if PUBLIC/... start a new one)
         case ePROCEDURE:  transFunc(scope, false);  continue;
         case eFUNCTION:   transFunc(scope, true);   continue;
         case eCONSTRUCTOR:dst<<QMARK; transFunc(scope, false, className);   continue;
         case eDESTRUCTOR: dst<<QMARK; transFunc(scope, false, className);   continue;
         case ePROPERTY:   transProperty();  continue; 
         default:          putBackToken();   break;
         }
      }
      if(    peekNextIsIdentifier()
          || ( scope==eFuncParam && (sTokPending==eCONST||sTokPending==eVAR||sTokPending==eOUT) )
        )
      {
         EToken const prefix = sTokPending;
         if( prefix==eCONST || prefix==eVAR || prefix==eOUT ) nextToken();
         ids.clear();

         id = requireIdentifier();

         {  pdstRedirect temp(00);

            for(;;) { // loop to allow multiple parameters with the same type (as is allowed in pascal parameter lists)
                    if( prefix==eCONST ) id = "const& "+id;
               else if( prefix==eVAR   ) id = "& "+id;
               else if( prefix==eOUT   ) id = "&/*OUT-only*/ "+id; //support added 20060701 based on contribution by Guru Kathiresan

               ids.push_back(id);
               if( tryGetToken(eComma) )
                  id = requireIdentifier();
               else
                  break;
            }

            requireToken(eColon);
            decl = readTypeDecl();
         }
         if( tryGetToken(eEqual) ) // default-value (for parameter list)
         {
            decl += " = ";
            EType et;
            decl += readExpr(et); //BUG/TODO: support init-value such as:  someConstOrVar: TGUID = (D1: $4657278B; D2: $411B:.....
         }

         for( unsigned i = 0 ; i!=ids.size() ; ++i ) {
            if( i>0 )
               dst << ( (scope == eFuncParam) ? ", " : "; " );
            dst << makeTypeDecl( decl, ids[i] );
         }

         if( scope != eFuncParam ) {
            if( peekToken()!=eEND ) // last semic is optional before eEND
               requireToken(eSemic);
            dst << ";";
         }
         else {
            if( tryGetToken(eSemic) ) //Q should require following param consistency ?
              dst << ", ";
         }
         continue;
      }
      break; // no option was handled here...
   }
}



/// Translate a Pascal TYPE declaration section.
void transTypeSection(EDeclScope /*scope*/)
{
   string name, base;
   ostream& dst = *pdst;

   while( peekNextIsIdentifier() )
   {
      {
         pdstRedirect temp(00);

         name = requireScopedIdentifier(); //NB: Delphi does not allow scoped identifiers here, but it's fine with us
         requireToken(eEqual);
         peekToken(); // to read next token within our null redirect.
      }

      if( tryGetToken(ePACKED)/*skip*/, tryGetToken(eRECORD) ) {
         cout << "Record Decl: " << name << endl;
         dst << "struct " << name << " {";
         transVarSection(eInRecord);
         //TODO: handle case switches --> union ??
         dst << "};";
         requireToken(eEND);
         requireToken(eSemic);
         continue;
      }
      bool isInterface = tryGetToken(eINTERFACE);
      if( isInterface || tryGetToken(eCLASS) ) {
        std::string const desc = ( isInterface ? "Interface" : "Class" );
        if( tryGetToken(eSemic) ) // forward-declaration
        {
          cout << desc<<" Forward-Decl: "<<name<<endl;
          if(isInterface)
            dst<< "\nclass/*interface*/ "<<name<<";";
          else
            dst<< "\nstruct "<<name<<";";
          continue;
        }
        if( tryGetToken(eOF) ) //20060701: parse statements like:   a_name = class of(a_class);
        {
           string const a_class = requireIdentifier();
           requireToken(eSemic);
           dst << QMARK"/* " << name << " = class of "<<a_class<<" -- needs to be replaced maybe by a template factory function? */";
           continue;
        }

        cout << desc<<" Decl: " << name << endl;
        {
            pdstRedirect temp(00);
            if( tryGetToken(eParIn) ) {
               base = requireScopedIdentifier();
               while( tryGetToken(eComma) )
                  base.append(", public "), base.append(requireScopedIdentifier());
               requireToken(eParOut);
            }
            else if(!isInterface)
               base = "TObject";
         }
         dst<< "\nclass"<<(isInterface ? "/*interface*/" : "" ) <<" " << name;
         if( !base.empty() )  dst << ": public " << base;
         if( tryGetToken(eSemic) ) { // empty definition e.g. MyError = class(Exception);  -- reported by Matt Minnis 20040409
            dst << "{ };";
            continue;
         }
         dst << "\n{";
         dst << "public:"; // is the default in Delphi, we add it explicitly if not provided
         if( ! tryGetToken(ePUBLIC) ) dst<<'\n';
         transVarSection(isInterface?eInInterf:eInClass,name);
         requireToken(eEND);
         requireToken(eSemic);
         dst<< "\n};\n";
         continue;
      }
      /*else*/ {
         dst << "typedef " << makeTypeDecl(readTypeDecl(), name);
         requireToken(eSemic);
         dst << ";";
      }
   }
}


/** Read the type signature of a function or procedure.
@param isFunc  true if a non-void return type is to be retrieved.
@param retTypeSig  OPTIONAL pointer to a string to be set to the function's return type.
*/
string readFuncSig(bool const isFunc, bool const isCtrOrDtr, string* retTypeSig)
{
   ostringstream dst(ios_base::out|ios_base::binary);
   pdstRedirect temp(dst);
   if( tryGetToken(eParIn) ) {
      dst << "(";
      transVarSection(eFuncParam);
      requireToken(eParOut);
      dst << ")";
   }
   else dst << "()";
   std::string const params = TMARK + dst.str();

   string decl;
   if( isFunc ) {
      pdstRedirect temp(00);
      if( tryGetToken(eColon) )
         decl = readTypeDecl();
      else
         decl = "/* MISSING PARAM/RESULT */" QMARK " " TMARK;
   }
   else
      decl = isCtrOrDtr ? TMARK : "void "TMARK;

   if( !! retTypeSig ) *retTypeSig = decl;
   return makeTypeDecl( decl, params );
}


/** Translate a function (or pascal procedure).
@param scope  Gives information on the context of the function's definition.
@param isFunc true if the function has a non-void return value.
@param i_className If non-empty, indicates that the function is a constructor or destructor.
*/
void transFunc(EDeclScope scope, bool isFunc, std::string const& i_className )
{
   if( scope==eFuncBody )
      *pdst << QMARK "/*SUB-FUNCTION TO BE EXTRACTED*/"; //fail( "Sub-functions cannot be translated" );

   ScopedSet<bool> stateTemp(sInFunc, isFunc);

   string className = i_className; // Local copy that may be edited. If non-empty, will indicate that the function is a constructor or a destructor
   string name, retType;
   {
      pdstRedirect discardBlanks(00);
      name = requireScopedIdentifier();
      if( className==QMARK ) {
         //NB: here I *really* miss not using perl/python, or at least linking in a regexp library...
         string::size_type const scopeMarkB = name.rfind(':');
         if( scopeMarkB==string::npos ) className.clear();
         else {
            string::size_type const scopeMarkA = name.rfind(':',scopeMarkB-2);
            //NB: a value of npos (equivalent to -1) is "ok" for scopeMarkA here (yes, ugly but valid unsigned math)
            className = name.substr(scopeMarkA+1,scopeMarkB-scopeMarkA-2);
         }
      }
      if( ! className.empty() ) {
         if     ( 0==my_stricmp(name.substr(name.size()-6),"create" ) ) name.replace(name.size()-6,string::npos,    className);
         else if( 0==my_stricmp(name.substr(name.size()-7),"destroy") ) name.replace(name.size()-7,string::npos,"~"+className);
      }
   }
   string decl = readFuncSig( isFunc, !className.empty(), &retType );

   requireToken(eSemic);

   string blanksStr;
   bool hasBody;
   {
      ostringstream blanksCapture(ios_base::out|ios_base::binary);
      pdstRedirect redirect(blanksCapture);
      string prefix, suffix;
      if( scope==eInInterf )
         suffix += " =0";

      hasBody = (scope==eUnitImpl || scope==eFuncBody);
      for(;;)
      {
         if( tryGetToken(eOVERLOAD) )
         {
            suffix += "/*overload*/";
         }
         else if( tryGetToken(eOVERRIDE) )
         {
            suffix += "/*override*/";
         }
         else if( tryGetToken(eREINTRODUCE) )
         {
            suffix += "/*Reintroduce*/";
         }
         else if( tryGetToken(eFAR) )
         {
            suffix += "/*Far*/";
         }
         else if( tryGetToken(eABSTRACT) )
         {
            if(scope!=eInInterf) suffix += "=0";
         }
         else if( tryGetToken(eFORWARD) )
         {
            hasBody = false;
         }
         else if( tryGetToken(eMESSAGE) )
         {
            suffix += "/* MESSAGE "+std::string(requireIdentifier())+" */";
         }
         else if( tryGetToken(eVIRTUAL) || tryGetToken(eDYNAMIC) )
         {
            prefix += "virtual ";
         }
         else if( tryGetToken(eSAFECALL) )  { prefix = "/*__safecall*/ " + prefix; }
         else if( tryGetToken(eSTDCALL ) )  { prefix = "/*__stdcall*/ "  + prefix; }
         else break;
         tryGetToken(eSemic);
      }

      *redirect.prev() << prefix << makeTypeDecl(decl,name) << suffix;
      blanksStr = blanksCapture.str();
   }

   cout << (isFunc?"Func ":"Proc ") << (hasBody?"impl: ":"decl: ") << name << endl;
   if( !hasBody ) {
      *pdst << ';' << blanksStr;
      return;
   }

   *pdst << blanksStr << '{';
   if( isFunc )
      *pdst << "   " << makeTypeDecl(retType,"result") << ';'; // declare result variable

   transDecls(eFuncBody);

   if( tryGetToken(eASM) )
   {
      *pdst << ";"QMARK"/* ASM";
      while( nextToken() != eEND ) *pdst << delphi_yytext;
      *pdst << "ASM END */";
      requireToken(eSemic);
      return;
   }

   requireToken(eBEGIN);
   transStatements();
   requireToken(eEND);
   requireToken(eSemic);
   if( isFunc )
      *pdst << "return result;\n";
   *pdst << "}";
}


/// Translate a Delphi resourcestring section into a comment block.
void transResourceString(EDeclScope scope)
{
   *pdst << "#if 0//resourcestring " QMARK ;
   transConstSection(scope);
   *pdst << "\n#endif\n";
}


/// Translate a declaration block (e.g. unit interface/implementation, class body, etc).
void transDecls(EDeclScope scope)
{
   for(;;) {
      switch( nextToken() )
      {
         case eUSES:       transUsesSection(scope);   continue;
         case eLABEL:      transLabelSection(scope);  continue;
         case eCONST:      transConstSection(scope);  continue;
         case eTHREADVAR: *pdst<<QMARK "/*THREADVAR*/";
         case eVAR:        transVarSection(scope);    continue;
         case eTYPE:       transTypeSection(scope);   continue;
         case eCLASS: { //200609: support for repeating "CLASS" procedure/function in the implementation (contributed by A. Tompe)
                           EToken tempToken = peekToken();
                           if ( tempToken == ePROCEDURE || tempToken == eFUNCTION )
                           {
                              *pdst << "static ";
                              putBackToken();
                           }
                           continue;
                       }
         case ePROCEDURE:  transFunc(scope,false);    continue;
         case eFUNCTION:   transFunc(scope,true);     continue;
         case eRESOURCESTRING:
                           transResourceString(scope); continue;
         case eCONSTRUCTOR:*pdst<<QMARK; transFunc(scope, false, QMARK);   continue;
         case eDESTRUCTOR: *pdst<<QMARK; transFunc(scope, false, QMARK);   continue;

         default:          putBackToken();           return;
      }
   }
}


/// Check if input is a program, return true and translate it.
bool tryTransProgram()
{
   ostringstream cmtDst(ios_base::out|ios_base::binary);
   pdstRedirect keepComments(cmtDst);
   bool isProgram = tryGetToken(ePROGRAM);
   if( ! isProgram ) {
      hdst << cmtDst.str();
      return false;
   }
   hdst << "\n\n//  THIS FILE IS EMPTY  because the matching .cpp is the main program\n\n";
   pdst = &cdst;
   cdst << cmtDst.str();

   cdst << "/* PROGRAM";
   cdst << requireIdentifier();
   requireToken(eSemic);
   cdst << " */";
   transDecls(eUnitImpl);

   requireToken(eBEGIN);
   cdst << "int main()\n{";
   transStatements();
   requireToken(eEND);
   requireToken(eDot);
   cdst << "   return 0;\n}\n";
   return true;
}


/// Translate a complete unit.
void transUnit()
{
   pdst = &hdst;
   requireToken( eUNIT );
   cout << "Unit name: "; cout << requireIdentifier() << endl;
   requireToken( eSemic );
   requireToken( eINTERFACE );
   cout << "--------------- PROCESSING UNIT INTERFACE\n";
   transDecls(eUnitInterf);
   requireToken( eIMPLEMENTATION );
   cout << "--------------- PROCESSING UNIT IMPLEMENTATION\n";
   pdst = &cdst;
   transDecls(eUnitImpl);

   if( tryGetToken(eINITIALIZATION) ) {
      cdst << "\n#if 0 //INITIALIZATION\n";
      transStatements();
      cdst << "\n#endif//INITIALIZATION\n";
   }
   if( tryGetToken(eFINALIZATION) ) {
      cdst << "\n#if 0 //FINALIZATION\n";
      transStatements();
      cdst << "\n#endif//FINALIZATION\n";
   }
   cout << "--------------- COMPLETED UNIT PROCESSING\n";
   requireToken(eEND);
   requireToken(eDot);
}


/// Main program.
int main(int argc, char* argv[])
{
   if(argc<2) {
      cerr << "Usage: "<<(argc?argv[0]:"delphi2cpp")<<" srcPath.pas [ srcPath2.pas ]...\n";
      return 0;
   }

   // Global Configuration
   addIDFilterTable(baseIDFilterTable);

 for(int indFile=1;indFile<argc;++indFile) {
   char const* const srcPath = argv[indFile];

#ifdef _CRT_INSECURE_DEPRECATE //Microsoft-specific extension -> use secure version to avoid "deprecated" warning
   if( !!fopen_s( &delphi_yyin, srcPath, "r" ) ) delphi_yyin=00;
#else
   delphi_yyin = fopen( srcPath, "r" );
#endif
   if( ! delphi_yyin ) {
     cerr << "Could not open source file: " << srcPath << endl;
     return 1;
   }
   sTokPendingIsValid = false;

   hdst.open( changeFileExt(srcPath,".h").c_str() );
   if( ! hdst.is_open() ) {
     cerr << "Could not open destination file: " << changeFileExt(srcPath,".h") << endl;
     return 1;
   }

   cdst.open( changeFileExt(srcPath,".cpp").c_str() );
   if( ! cdst.is_open() ) {
     cerr << "Could not open destination file: " << changeFileExt(srcPath,".cpp") << endl;
     return 1;
   }

   std::string errTxt;
   try {
      std::string guard = "INCLUDED_"+toupper(baseName(srcPath))+"_H";
      hdst<<"/**   @file  \n"
         "     @brief  \n"
         "*/\n"
         "#pragma once\n"
         "#ifndef "<<guard<<"\n"
         "#define "<<guard<<"\n";
      cdst<<"/**   @file  \n"
         "     @brief  \n"
         "*/\n"
         "#include \"" << baseName(srcPath) << ".h\"\n\n";

      if( ! tryTransProgram() )
         transUnit();

      cdst<<"\n\n//END\n";
      hdst<<"\n\n#endif//"<<guard<<"\n//END\n";
   }
   catch( std::exception& x ) { errTxt = x.what(); }
   catch(...)                 { errTxt = "***UNKNOWN ERROR***"; }

   fclose(delphi_yyin);
   hdst.close();
   cdst.close();

   if( ! errTxt.empty() )
   {
      cerr << "\n\nERROR ENCOUNTERED: " << errTxt
           << "\n on line "<<delphi_yylineno<<" at token '"
           << (delphi_yytext?delphi_yytext:"*unknown*")<<"'.\n\n";
   }

 }
 return 0;
}


