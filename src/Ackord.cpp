#include "Ackord.h"

std::string TMainF::AnalyzeChord(){
  std::string result;
  int I;
  unsigned char Key;
  unsigned char K2;
  unsigned char BNote;
  result = "";

  // No note
  if(KeyMatch([], Key)){return result;}

  // Only one note
  if(KeyMatch([0], Key)){
    result = NoteNames[Key];
    return result;
  }

  // Two note stuff,everything except the power chord
  if(KeyMatch([0,1], Key)
  || KeyMatch([0,2], Key)
  || KeyMatch([0,3], Key)
  || KeyMatch([0,4], Key)
  || KeyMatch([0,6], Key)){
    result = NoteNames[LowestNoteOct];
    for(I = 1 ; I < 12 ; ++I){
      if(NoteOct[(LowestNoteOct + I) % 12] != 0){
        result = result + ","+NoteNames[(LowestNoteOct+I) % 12];
      }
    }
    return result;
  }

  // Normal major chords.
  if(KeyMatch([0,4,7], Key)){ result = NoteNames[Key];}

  // Normal minor chords.
  else if(KeyMatch([0,3,7], Key)){ result = NoteNames[Key]+"m";}

  // sus4,or 2
  else if(KeyMatch([0,5,7], Key)){
    BNote = GetRelation(Key ,LowestNoteOct);
    if(BNote == 5){
      Key = LowestNoteOct;
      result = NoteNames[Key]+"2";
    }
    else{
      result = NoteNames[Key]+"sus";
    }

  // 7th
  else if(KeyMatch([0,4,7,10], Key)){
    BNote = GetRelation(Key ,LowestNoteOct);
    // 7th in base,express as e.g. C/Bb
    if(BNote == 10) result = NoteNames[Key];else
      result = NoteNames[Key]+"7";}
  else if(KeyMatch([0,4,10], Key)) result = NoteNames[Key]+"7";

  // m7th
  else if(KeyMatch([0,3,7,10], Key)){
    BNote = GetRelation(Key ,LowestNoteOct);
    // 7th in base,express as e.g. Cm/Bb
    if(BNote == 10) result = NoteNames[Key]+"m";
    // 3rd in base,express as 6
    else if(BNote == 3){
      Key = LowestNoteOct;
      result = NoteNames[Key]+"6";}
    else
      result = NoteNames[Key]+"m7";}

  // m b5
  else if(KeyMatch([0,3,6], Key)) result = NoteNames[Key]+"m b5";
  // b5
  else if(KeyMatch([0,4,6], Key)) result = NoteNames[Key]+" b5";
  // maj7
  else if(KeyMatch([0,4,7,11], Key)) result = NoteNames[Key]+"maj7";
  // m maj7
  else if(KeyMatch([0,3,7,11], Key)) result = NoteNames[Key]+"m maj7";
  // maj9
  else if(KeyMatch([0,2,4,7,11], Key)) result = NoteNames[Key]+"maj9";
  // 6 9
  else if(KeyMatch([0,2,4,7,9], Key)){
    BNote = GetRelation(Key ,LowestNoteOct);
    // 6th in base,express as m7 add11
    if(BNote == 9){
      Key = LowestNoteOct;
      result = NoteNames[Key]+"m7 add11";}
    // 9th in base,express as 11 add5
    else if(BNote == 2){
      Key = LowestNoteOct;
      result = NoteNames[Key]+"11 add5";}
    else
      result = NoteNames[Key]+" 6 9";}
  // m7b5
  else if(KeyMatch([0,3,6,10], Key)){
    BNote = GetRelation(Key ,LowestNoteOct);
    // 3th in base,express as m6
    if(BNote == 3){
      Key = LowestNoteOct;
      result = NoteNames[Key]+"m6";}
    else
      result = NoteNames[Key]+"m7b5";}
  // sus4 + 7th
  else if(KeyMatch([0,5,7,10], Key)) result = NoteNames[Key]+"sus7";
  // Power chord
  else if(KeyMatch([0,7], Key)) result = NoteNames[Key]+"no3";
  // Dim
  else if(KeyMatch([0,3,6,9], Key)){
    // Check which one is lowest
    K2 = Key;
    for(I = 1 ; I < 4 ; ++I){
      if(LowerNote(K2 ,Key+I*3)){
        K2 = (Key+I*3) % 12;
      }
    }
    Key = K2;
    result = NoteNames[K2]+char(0xB0);
  }
  // Major #5
  else if(KeyMatch([0,4,8], Key)){
    // Check which one is lowest
    K2 = Key;
    for(I = 1 ; I < 3 ; ++I){
      if(LowerNote(K2 ,Key+I*4)){
        K2 = (Key+I*4) % 12;
      }
    }
    Key = K2;
    result = NoteNames[K2]+"+";
  }
  // add9
  else if(KeyMatch([0,2,4,7], Key)){
    BNote = GetRelation(Key ,LowestNoteOct);
    // 9 in base,express as 11th-chord
    if(BNote == 2){
      Key = LowestNoteOct;
      result = NoteNames[Key]+"11";}
    else
      result = NoteNames[Key]+"add9";}
  // 9
  else if((KeyMatch([0,2,4,7,10], Key))){
    BNote = GetRelation(Key ,LowestNoteOct);
    // 7th in base,express as add9,e.g. Cadd9/Bb
    if((BNote == 10)){
      result = NoteNames[Key]+"add9";
    }
    else{
      result = NoteNames[Key]+"9";
    }
  }
  else if(KeyMatch([0,2,4,10], Key)) result = NoteNames[Key]+"9";
  // m9
  else if(KeyMatch([0,2,3,7,10], Key)) result = NoteNames[Key]+"m9";
  // m11
  else if(KeyMatch([0,2,3,5,7,10], Key)
       || KeyMatch([0,2,3,5,10], Key))
         result = NoteNames[Key]+"m11";
  // m 6 9
  else if((KeyMatch([0,2,3,7,9], Key))) result = NoteNames[Key]+"m 6 9";
  // madd9
  else if((KeyMatch([0,2,3,7], Key))) result = NoteNames[Key]+"m add9";
  // 7b9
  else if((KeyMatch([0,1,4,7,10], Key))
       || (KeyMatch([0,1,4,10], Key)))
         result = NoteNames[Key]+"7b9";
  // 7#9
  else if((KeyMatch([0,3,4,7,10], Key))
       || (KeyMatch([0,3,4,10], Key)))
         result = NoteNames[Key]+"7#9";
  // 7b5
  else if((KeyMatch([0,4,6,10], Key)))
    result = NoteNames[Key]+"7b5";
  // 7#5
  else if((KeyMatch([0,4,8,10], Key))){
    BNote = GetRelation(Key ,LowestNoteOct);
    // 7 in base,express as 9#11 no3
    if(BNote == 10){
      Key = LowestNoteOct;
      result = NoteNames[Key]+"9#11 no3";}
    else
      result = NoteNames[Key]+"7#5";}
  // 7#5#9
  else if((KeyMatch([0,3,4,8,10], Key)))
    result = NoteNames[Key]+"7#5#9";
  // 7b5#9
  else if((KeyMatch([0,3,4,6,10], Key)))
    result = NoteNames[Key]+"7b5#9";
  // 7b5b9
  else if((KeyMatch([0,1,4,6,10], Key)))
    result = NoteNames[Key]+"7b5b9";
  // 7#5b9
  else if((KeyMatch([0,1,4,8,10], Key))){
    BNote = GetRelation(Key ,LowestNoteOct);
    // 7 in base,express as m9 b5
    if(BNote == 10){
      Key = LowestNoteOct;
      result = NoteNames[Key]+"m9 b5";}
    else
      result = NoteNames[Key]+"7#5b9";}
  // 13 (with and without both 5th and 9th)
  else if(KeyMatch([0,4,7,9,10], Key)
       || KeyMatch([0,4,9,10], Key)
       || KeyMatch([0,2,4,7,9,10], Key)
       || KeyMatch([0,2,4,9,10], Key))
         result = NoteNames[Key]+"13";
  // maj13 (with and without both 5th and 9th)
  else if(KeyMatch([0,4,7,9,11], Key)
       || KeyMatch([0,4,9,11], Key)
       || KeyMatch([0,2,4,7,9,11], Key)
       || KeyMatch([0,2,4,9,11], Key))
         result = NoteNames[Key]+"maj 13";
  // 13#11
  else if((KeyMatch([0,2,4,6,9,10], Key))
       || (KeyMatch([0,2,4,6,7,9,10], Key)))
         result = NoteNames[Key]+"13#11";
  // 13#11 no3
  else if((KeyMatch([0,2,6,9,10], Key))
       || (KeyMatch([0,2,6,7,9,10], Key)))
         result = NoteNames[Key]+"13#11 no3";
  // 13#9
  else if((KeyMatch([0,3,4,9,10], Key)))
    result = NoteNames[Key]+"13#9";
  // 13b9
  else if((KeyMatch([0,1,4,9,10], Key)))
    result = NoteNames[Key]+"13b9";
  // 6 no3
  else if((KeyMatch([0,7,9], Key))){
    BNote = GetRelation(Key ,LowestNoteOct);
    // 6 in base,express as m7 no5
    if(BNote == 9){
      Key = LowestNoteOct;
      result = NoteNames[Key]+"m7 no5";}
    else
      result = NoteNames[Key]+"6 no3";}
  // 7 no3
  else if((KeyMatch([0,7,10], Key))){
    BNote = GetRelation(Key ,LowestNoteOct);
    // 5th in base,express as m no5 add11
    if(BNote == 7){
      Key = LowestNoteOct;
      result = NoteNames[Key]+"m no5 add11";}
    else
      result = NoteNames[Key]+"7 no3";}
  // 9#11
  else if((KeyMatch([0,2,4,6,10], Key)))
    result = NoteNames[Key]+"9#11";
  // m9#11
  else if((KeyMatch([0,2,3,6,10], Key)))
    result = NoteNames[Key]+"m9#11";
  // add9 no5
  else if((KeyMatch([0,2,4], Key)))
    result = NoteNames[Key]+"add9 no5";
  // m add9 no5
  else if((KeyMatch([0,2,3], Key)))
    result = NoteNames[Key]+"m add9 no5";
  // m9 no5
  else if((KeyMatch([0,2,3,10], Key)))
    result = NoteNames[Key]+"m9 no5";
  // m add11
  else if((KeyMatch([0,3,5,7], Key)))
    result = NoteNames[Key]+"m add11";
  // add11
  else if((KeyMatch([0,4,5,7], Key))){
    BNote = GetRelation(Key ,LowestNoteOct);
    // 11 in base
    if(BNote == 5){
      result = NoteNames[Key];
    }
    else{
      result = NoteNames[Key]+"add11";
    }
  // mb5 add11
  else if((KeyMatch([0,3,5,6], Key)))
    result = NoteNames[Key]+"mb5 add11";
  // maj7 no5
  else if((KeyMatch([0,4,11], Key)))
    result = NoteNames[Key]+"maj7 no5";
  // m maj7 no5
  else if((KeyMatch([0,3,11], Key)))
    result = NoteNames[Key]+"m maj7 no5";
  // maj7 no3
  else if((KeyMatch([0,7,11], Key)))
    result = NoteNames[Key]+"maj7 no3";
  else{ // unknown chord
    result = "???";
    return result;
  }

  // See if we have the right base. If not,add it.
  if(Key != LowestNoteOct){
    result = result + "/"+NoteNames[LowestNoteOct];
  }

  return result;
}


bool TMainF::Matches(unsigned char Key, unsigned long Match){
  bool result;
  int I;

  long I_end = High(NoteKind)+1 ;
  for(I = 0 ; I < I_end ; ++I){
    if((I in Match) != (NoteOct[(Key + I) % 12] != 0)){
      result = false;
      return result;
    }
  }
  result = true;
  return result;
}

bool TMainF::KeyMatch(unsigned long Match,  unsigned char & Key){
  bool result;
  int I;

  long I_end = High(NoteKind)+1;
  for(I = 0 ; I < I_end ; ++I){
    if(Matches(I ,Match)){
      result = true;
      Key = I;
      return result;
    }
  }
  result = false;
  return result;
}

bool TMainF::LowerNote(unsigned char Key1,unsigned char Key2){
  bool result;
  Key1 = Key1 % 12;
  Key2 = Key2 % 12;

  if(NoteOct[Key2] == 0){ result = false;}
  else if(NoteOct[Key1] == 0){ result = true;}
  else if(NoteOct[Key1] > NoteOct[Key2]){ result = true;}
  else if(NoteOct[Key1] < NoteOct[Key2]){ result = false;}
  else{result = Key2 < Key1;} // Same octave,then we come here
  
  return result;
}

unsigned char TMainF::GetRelation(unsigned char Key,unsigned char RelatedKey){
  unsigned char result;
  int I;

  I = (RelatedKey % 12) - (Key % 12);
  if(I < 0){ I = I + 12;}
  Assert((I >= 0) && (I < 12));
  result = I;
  return result;
}

