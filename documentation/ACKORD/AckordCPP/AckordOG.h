/**   @file  
     @brief  
*/
#pragma once
#ifndef INCLUDED_ACHORD_H
#define INCLUDED_ACHORD_H
 

/*
This file is part of the program "Ackord", originally written by David Henningsson.

    Ackord is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Ackord is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Ackord; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/





/*$WARN SYMBOL_DEPRECATED OFF*/


  
#include "Windows.h" 
#include "Messages.h" 
#include "SysUtils.h" 
#include "Variants.h" 
#include "Classes.h" 
#include "Graphics.h" 
#include "Controls.h" 
#include "Forms.h"
  
#include "Dialogs.h" 
#include "Mmsystem.h" 
#include "ExtCtrls.h" 
#include "StdCtrls.h"


  static double/*?*/ const NoteKindSize = 12;


  typedef int/*?*//*range: 0 .. NoteKindSize-1 */ NoteKind; // 0 = C, 1 = C#, 2 = D etc
  typedef unsigned long/*?*//*set of: NoteKind */ NoteKindSet;


  static std::string const NoteNames[/*?*/ range of NoteKind ] = ( "C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "H" );



  
struct/*class*/ TMainF: public TForm
    
{TPanel AckordPanel;
    TButton SettingsBtn;
    void FormCreate(TObject Sender);
    void FormDestroy(TObject Sender);
    void FormKeyDown(TObject Sender,   unsigned short & Key, 
      TShiftState Shift);
    void SettingsBtnClick(TObject Sender);
    void AckordPanelResize(TObject Sender);
  
private:
    // Window handle that will receive midi message
    HWnd MidiWindow;
    // Handles for midi inputs
    HMIDIIN DevHandles[ /*?*/ ];
    // Handles for midi thru
    HMIDIOut ThruHandles[ /*?*/ ];
    // True if the sustain pedal is currently pressed
    bool SustainPressed;
    // This array contains whether a key is pressed or not.
    // 2 = Pressed, 1 = not pressed but active due to sustain
    // 0 = not pressed.
    unsigned char NoteArray[128];
    // This array contains the lowest octave (1-11) that has this key (C - B) pressed. 0 = not pressed
    unsigned char NoteOct[/*?*/ range of NoteKind ];
    // Which note is the lowest one? (LowestOct = Lowest mod 12)
    unsigned char LowestNote; unsigned char LowestNoteOct;

    // Called whenever a midi message is received
    void MidiWndProc( TMessage & Msg);
    // Uses information in NoteArray and NoteOct to construct a chord.
    std::string AnalyzeChord();
    // Returns true if Key2 is pressed lower than Key1 (to the left on the keyboard)
    bool LowerNote(unsigned char Key1, unsigned char Key2);
    // For which key is there an exact match? Returns false if none.
    bool KeyMatch(NoteKindSet Match,   unsigned char & Key);
    // If there is an exact match in NoteOct for the match
    bool Matches(unsigned char Key,  NoteKindSet Match);
    // The distance (irrelevant of octave) between Key and Relatedkey (always positive).
    unsigned char GetRelation(unsigned char Key, unsigned char RelatedKey);
    // This one opens all open midi ports. Except for the first time - closemidi should have been called first.
    void OpenMidi();
    // This one closes all open midi ports.
    void CloseMidi();
  
public:
  
};



  TMainF MainF;



#endif//INCLUDED_ACHORD_H
//END
