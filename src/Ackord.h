#pragma once
#ifndef ACHORD_H
#define ACHORD_H

typedef int NoteKind; // 0 = C, 1 = C#, 2 = D etc
static std::string const NoteNames[ range of NoteKind ] = 
  ( "C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B" );

struct TMainF: public TForm{
  
  private:
    bool SustainPressed;

    // This array contains whether a key is pressed or not.
    // 2 = Pressed, 1 = not pressed but active due to sustain, 0 = not pressed.
    unsigned char NoteArray[128];

    // This array contains the lowest octave (1-11) that has this key (C - B) pressed. 0 = not pressed
    unsigned char NoteOct[ range of NoteKind ];

    // Which note is the lowest one? (LowestOct = Lowest mod 12)
    unsigned char LowestNote; 
    unsigned char LowestNoteOct;

    // Uses information in NoteArray and NoteOct to construct a chord.
    std::string AnalyzeChord();

    // Returns true if Key2 is pressed lower than Key1 (to the left on the keyboard)
    bool LowerNote(unsigned char Key1, unsigned char Key2);

    // For which key is there an exact match? Returns false if none.
    bool KeyMatch(unsigned long Match,   unsigned char & Key);

    // If there is an exact match in NoteOct for the match
    bool Matches(unsigned char Key,  unsigned long Match);

    // The distance (irrelevant of octave) between Key and Relatedkey (always positive).
    unsigned char GetRelation(unsigned char Key, unsigned char RelatedKey);
  
  public:
};

#endif

