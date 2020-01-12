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
/* Scanner to convert pascal source code c++ */
/* flex reference:  http://www.gnu.org/manual/flex-2.5.4/html_mono/flex.html

Note: for compatibility with standard C++, the output of flex is
      post-processed using a perl script, which is attached: flex_MSVC

Compilation command:
      flex_MSVC  -odelphi_yy.cpp  delphi_yy.l
*/

%pointer
%option prefix="delphi_yy"
%option never-interactive
%option noyywrap
%option case-insensitive
%option 8bit
%option yylineno

%{

#include "delphi_yy.h"

#define YY_DECL EToken delphi_yylex()

void fail(char const* msg);
#define YY_FATAL_ERROR(msg) fail( "flex failure: " msg )

%}


DIG   [0-9]
HEX   [0-9A-F]

STR   (\'([^\']|\'\')*\')
ID    ([A-Z_][A-Z_0-9]*)
CMTB  ("{"[^\}]*"}"|"(*"([^\*]|\*[^\)])+"*)")
CMTL  ("//"[^\n\r]*)

INT   ({DIG}+|\${HEX}+)
FLT   ({DIG}+(\.{DIG}+)?([eE][+-]?{DIG}+)?)

 /*   ANY   ([^\0]|{STR}|{CMT})*   */

%%

 /* pascal keywords */
"AND"             return eAND;
"DOWNTO"          return eDOWNTO;
"IN"              return eIN;
"OR"              return eOR;
"STRING"          return eSTRING;
"ARRAY"           return eARRAY;
"ELSE"            return eELSE;
"INHERITED"       return eINHERITED;
"OUT"      return eOUT;
"THEN"      return eTHEN;
"AS"      return eAS;
"END"      return eEND;
"INITIALIZATION"      return eINITIALIZATION;
"PACKED"      return ePACKED;
"THREADVAR"      return eTHREADVAR;
"ASM"      return eASM;
"EXCEPT"      return eEXCEPT;
"INLINE"      return eINLINE;
"PROCEDURE"      return ePROCEDURE;
"TO"      return eTO;
"BEGIN"      return eBEGIN;
"EXPORTS"      return eEXPORTS;
"INTERFACE"      return eINTERFACE;
"PROGRAM"      return ePROGRAM;
"TRY"      return eTRY;
"CASE"      return eCASE;
"FILE"      return eFILE;
"IS"      return eIS;
"PROPERTY"      return ePROPERTY;
"TYPE"      return eTYPE;
"CLASS"      return eCLASS;
"FINALIZATION"      return eFINALIZATION;
"LABEL"      return eLABEL;
"RAISE"      return eRAISE;
"UNIT"      return eUNIT;
"CONST"      return eCONST;
"FINALLY"      return eFINALLY;
"LIBRARY"      return eLIBRARY;
"RECORD"      return eRECORD;
"UNTIL"      return eUNTIL;
"CONSTRUCTOR"      return eCONSTRUCTOR;
"FOR"      return eFOR;
"MOD"      return eMOD;
"REPEAT"      return eREPEAT;
"USES"      return eUSES;
"DESTRUCTOR"      return eDESTRUCTOR;
"FUNCTION"      return eFUNCTION;
"NIL"      return eNIL;
"RESOURCESTRING"      return eRESOURCESTRING;
"VAR"      return eVAR;
"DISPINTERFACE"      return eDISPINTERFACE;
"GOTO"      return eGOTO;
"NOT"      return eNOT;
"SET"      return eSET;
"WHILE"      return eWHILE;
"DIV"      return eDIV;
"IF"      return eIF;
"OBJECT"      return eOBJECT;
"SHL"      return eSHL;
"WITH"      return eWITH;
"DO"      return eDO;
"IMPLEMENTATION"      return eIMPLEMENTATION;
"OF"      return eOF;
"SHR"      return eSHR;
"XOR"      return eXOR;

 /* keyword-like identifiers */
"ABSOLUTE"      return eABSOLUTE;
"DYNAMIC"      return eDYNAMIC;
"NAME"      return eNAME;
"PROTECTED"      return ePROTECTED;
"RESIDENT"      return eRESIDENT;
"ABSTRACT"      return eABSTRACT;
"EXPORT"      return eEXPORT;
"NEAR"      return eNEAR;
"PUBLIC"      return ePUBLIC;
"SAFECALL"      return eSAFECALL;
"ASSEMBLER"      return eASSEMBLER;
"EXTERNAL"      return eEXTERNAL;
"NODEFAULT"      return eNODEFAULT;
"PUBLISHED"      return ePUBLISHED;
"STDCALL"      return eSTDCALL;
"AUTOMATED"      return eAUTOMATED;
"FAR"      return eFAR;
"OVERLOAD"      return eOVERLOAD;
"READ"      return eREAD;
"STORED"      return eSTORED;
"CDECL"      return eCDECL;
"FORWARD"      return eFORWARD;
"OVERRIDE"      return eOVERRIDE;
"READONLY"      return eREADONLY;
"VIRTUAL"      return eVIRTUAL;
"CONTAINS"      return eCONTAINS;
"IMPLEMENTS"      return eIMPLEMENTS;
"PACKAGE"      return ePACKAGE;
"REGISTER"      return eREGISTER;
"WRITE"      return eWRITE;
"DEFAULT"      return eDEFAULT;
"INDEX"      return eINDEX;
"PASCAL"      return ePASCAL;
"REINTRODUCE"      return eREINTRODUCE;
"WRITEONLY"      return eWRITEONLY;
"DISPID"      return eDISPID;
"MESSAGE"      return eMESSAGE;
"PRIVATE"      return ePRIVATE;
"REQUIRES"      return eREQUIRES;
"RESULT"      return eRESULT;
"CONTINUE"      return eCONTINUE;
"BREAK"      return eBREAK;
"EXIT"      return eEXIT;
"ON"      return eON;    //ivec20060701 not listed as a reserved word or directive in the language reference I have, even though it does act as a keyword after try..except
 /* more names for special processing */
"TRUE"      return eTRUE;
"FALSE"     return eFALSE;
"INC"       return eINC;
"DEC"       return eDEC;
"ASSIGNED"  return eASSIGNED;

 /* some key compiler directives */

"{$IFDEF".*"}"   return ePRE_IFDEF;
"{$IFNDEF".*"}"  return ePRE_IFNDEF;
"{$ELSE".*"}"    return ePRE_ELSE;
"{$ENDIF".*"}"   return ePRE_ENDIF;

 /* all symbols and special characters */
[ \t]+      return eBlanks;
(\r?\n|\r)  yytext[0] = '\n'; yytext[1] = '\0'; yyleng = 1; return eBlanks;
{CMTB}      return eCommentBlock;
{CMTL}      return eCommentLine;
{STR}       return eCharString;
"#"         return eCharCoded;   /* was "#"{INT} */
{INT}       return eNumInt;
{FLT}       return eNumFloat;
{ID}        return eID;
"["         return eBrackIn;
"]"         return eBrackOut;
"("         return eParIn;
")"         return eParOut;
".."        return eRange;
"."         return eDot;
","         return eComma;
";"         return eSemic;
":"         return eColon;
"@"         return eAt;
"^"         return eCirc;
"+"         return ePlus;
"-"         return eMinus;
"*"         return eMul;
"/"         return eDiv;
":="        return eAssign;
"="         return eEqual;
"<>"        return eUnequal;
"<"         return eSmaller;
">"         return eLarger;
"<="        return eSmallerEq;
">="        return eLargerEq;
"^."        return eInto;

.           return eIllegal;
<<EOF>>     return eEOF;

%%





