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

namespace std {}
#include <cstdio>
using namespace std;


/// List of token identifiers for the input Pascal/Delphi source file
enum EToken
   { eIllegal = -2
   , eEOF = -1
   , eBlanks
   , eCharString
   , eCharCoded
   , eNumInt
   , eNumFloat
   , eCommentBlock
   , eCommentBlockClassic
   , eCommentLine
   , eID
   , eBrackIn
   , eBrackOut
   , eParIn
   , eParOut
   , eRange
   , eDot
   , eComma
   , eSemic
   , eColon
   , eAt
   , eCirc
   , ePlus
   , eMinus
   , eMul
   , eDiv
   , eAssign
   , eEqual
   , eUnequal
   , eSmaller
   , eLarger
   , eSmallerEq
   , eLargerEq
   , eInto // ^. combined is processed in a special way
   // key compiler directives
   , ePRE_IFDEF
   , ePRE_IFNDEF
   , ePRE_ELSE
   , ePRE_ENDIF
   // pascal keywords
   , eAND
   , eDOWNTO
   , eIN
   , eOR
   , eSTRING
   , eARRAY
   , eELSE
   , eINHERITED
   , eOUT
   , eTHEN
   , eAS
   , eEND
   , eINITIALIZATION
   , ePACKED
   , eTHREADVAR
   , eASM
   , eEXCEPT
   , eINLINE
   , ePROCEDURE
   , eTO
   , eBEGIN
   , eEXPORTS
   , eINTERFACE
   , ePROGRAM
   , eTRY
   , eCASE
   , eFILE
   , eIS
   , ePROPERTY
   , eTYPE
   , eCLASS
   , eFINALIZATION
   , eLABEL
   , eRAISE
   , eUNIT
   , eCONST
   , eFINALLY
   , eLIBRARY
   , eRECORD
   , eUNTIL
   , eCONSTRUCTOR
   , eFOR
   , eMOD
   , eREPEAT
   , eUSES
   , eDESTRUCTOR
   , eFUNCTION
   , eNIL
   , eRESOURCESTRING
   , eVAR
   , eDISPINTERFACE
   , eGOTO
   , eNOT
   , eSET
   , eWHILE
   , eDIV
   , eIF
// , eOBJECT -- handled as pseudo-keyword for this translation
   , eSHL
   , eWITH
   , eDO
   , eIMPLEMENTATION
   , eOF
   , eSHR
   , eXOR
   // keyword-like identifiers
   , eABSOLUTE
   , eDYNAMIC
   , eNAME
   , ePROTECTED
   , eRESIDENT
   , eABSTRACT
   , eEXPORT
   , eNEAR
   , ePUBLIC
   , eSAFECALL
   , eASSEMBLER
   , eEXTERNAL
   , eNODEFAULT
   , ePUBLISHED
   , eSTDCALL
   , eAUTOMATED
   , eFAR
   , eOBJECT   //NB: actually a real keyword
   , eOVERLOAD
   , eREAD
   , eSTORED
   , eCDECL
   , eFORWARD
   , eOVERRIDE
   , eREADONLY
   , eVIRTUAL
   , eCONTAINS
   , eIMPLEMENTS
   , ePACKAGE
   , eREGISTER
   , eWRITE
   , eDEFAULT
   , eINDEX
   , ePASCAL
   , eREINTRODUCE
   , eWRITEONLY
   , eDISPID
   , eMESSAGE
   , ePRIVATE
   , eREQUIRES
   , eRESULT
   , eCONTINUE
   , eBREAK
   , eEXIT
   , eON    //ivec20060701 not listed as a reserved word or directive in the language reference I have, even though it does act as a keyword after try..except
   // more words with special processing
   , eTRUE
   , eFALSE
   , eINC
   , eDEC
   , eASSIGNED
   , eFirstSemiKeyword = eABSOLUTE
   , eLastSemiKeyword = eASSIGNED
   };


extern FILE*   delphi_yyin;     ///< Pointer to the input Pascal/Delphi file
extern char*   delphi_yytext;   ///< C-string representation of the current identifier
extern int     delphi_yyleng;   ///< Length of the string at delphi_yytext.
extern int     delphi_yylineno; ///< Current line number within the source file
extern EToken  delphi_yylex();  ///< flex-generated function called internally to retrieve the next identifier.


