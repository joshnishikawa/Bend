# The keys in this list represent all valid major keys. The
# corresponding values represent the number of semitones above C0 each
# tonic is. This allows chords to be identified universally while
# still being able to reference specific notes and keys in the output.
maj_tonics = {
'C': 0,
'C#': 1, 'Db': 1,
'D': 2,
'Eb': 3,
'E': 4,
'F': 5,
'F#': 6, 'Gb': 6,
'G': 7,
'Ab': 8,
'A': 9,
'Bb': 10,
'B': 11, 'Cb': 11}

# Same as above but only for valid minor keys (AKA Aeolean modes).
# Other modes need to be handled at some point.
min_tonics = {
'C': 0,
'C#': 1,
'D': 2,
'D#': 3, 'Eb': 3,
'E': 4,
'F': 5,
'F#': 6,
'G': 7,
'G#': 8, 'Ab': 8,
'A': 9,
'A#': 10, 'Bb': 10,
'B': 11}

# This dict is meant to contain all valid note names and the number of
# semitones above CO. It does not currently include double-flats or
# double-sharps and I'm not even sure this list will be relevant.
notes = {
'B#': 0, 'C': 0,
'C#': 1, 'Db': 1,
'D': 2,
'D#': 3, 'Eb': 3,
'E': 4, 'Fb': 4,
'E#': 5, 'F': 5,
'F#': 6, 'Gb': 6,
'G': 7,
'G#': 8, 'Ab': 8,
'A': 9,
'A#': 10, 'Bb': 10,
'B': 11, 'Cb': 11}

# Indexes in this list correspond to the number of semitones above the
# tonic of any key. The items are the names of those degrees.
# This list needs to be more specific concerning major vs. minor and
# so-labelled 'passing notes'.
degrees = [
'the Tonic',
'a passing note',
'the Super Tonic',
'the Mediant',
'the Mediant',
'the Sub Dominant',
'a passing note',
'the Dominant',
'the Sub Mediant',
'the Sub Mediant',
'the Sub Tonic',
'the Leading Note']

# Any chord and its inversion (disregarding any particular voicing)
# can be identified by finding the intervals between the bass note
# (not the root) and all other notes in the chord.
# The keys in the 'chords' dict are lists of numbers corresponding to
# those intervals. Each number in the list is the number of semitones
# above the bass note that the note is. (bass note being an implied 0)
# Notes that are more than an octave above the root
# (9th, 11th, 13th, or 15th) are reduced to make for referencing.

chords = {
#==========intervals==================================================
(0,): ['a Unison'],
(1,): ['a Semitone'],
(2,): ['a Whole Tone'],
(3,): ['a Minor Third', 'an Augmented 9th',
      'a minor chord with implied 5th in root position'],
(4,): ['a Major Third', 'a major chord with an implied 5th'],
(5,): ['a Perfect Fourth'],
(6,): ['a Diminished Fifth', 'an Augmented Fourth', 'a tritone'],
(7,): ['a Perfect Fifth'],
(8,): ['a Minor Sixth', 'an Augmented Fifth',
      'a major chord with implied 5th in 1st inversion'],
(9,): ['a Major Sixth', 'a minor chord with implied 5th in root position'],
(10,): ['a Minor Seventh', 'an Augmented Sixth'],
(11,): ['a Major Seventh'],
(12,): ['an Octave'],

#==========3-tone chords==============================================
(7, 12): 'a Power Chord',
(4, 7): 'a Major Triad in root position', # Neapolitan
(3, 8): 'a Major Triad in 1st inversion',
(5, 9): 'a Major Triad in 2nd inversion',

(3, 7): 'a Minor Triad in root position',
(4, 9): 'a Minor Triad in 1st inversion',
(5, 8): 'a Minor Triad in 2nd inversion',

(3, 6): 'a Diminished Triad in root position',
(3, 8): 'a Diminished Triad in 1st inversion',
(6, 9): 'a Diminished Triad in 2nd inversion',

# Inversions for augmented triads can't be determined by intervals only.
(4, 8): 'an Augmented Triad', 

(2, 7): 'a Sus 2 in root position',
(5, 10): 'a Sus 2 in 1st inversion',
(5, 7): 'a Sus 2 in 2nd inversion',

(4, 5): 'a Sus 4 in root position',
(1, 3): 'a Sus 4 in 1st inversion',
(7, 11): 'a Sus 4 in 2nd inversion',

(4, 10): 'an Italian 6th chord',

#==========7th chords with implied 5ths===============================
(4, 11): 'Major 7th in root position with an implied 5th',
(7, 8): 'Major 7th in 1st inversion with an implied 5th',
# don't imply the bass note
(1, 5): 'Major 7th in 3rd inversion with an implied 5th',

(3, 10): 'Minor 7th in root position with an implied 5th',
(7, 9): 'Minor 7th in 1st inversion with an implied 5th',
(2, 5): 'Minor 7th in 3rd inversion with an implied 5th',

(7, 10): ['Dominant 7th in root position with an implied 5th',
          'German 6th with an implied 5th'], #FIXME this is wrong I think
(3, 8): 'Dominant 7th in 1st inversion with an implied 5th',
(2, 6): 'Dominant 7th in 3rd inversion with an implied 5th',

# don't imply a diminished 5th

(3, 11): 'Minor Major 7th in root position with an implied 5th',
(8, 9): 'Minor Major 7th in 1st inversion with an implied 5th',
(1, 4): 'Minor Major 7th in 3rd inversion with an implied 5th',
# don't imply an augmented 5th

#==========4-tone chords==============================================
(4, 7, 11): 'Major 7th in root position',
(3, 7, 8): 'Major 7th in 1st inversion',
(4, 5, 9): 'Major 7th in 2nd inversion',
(1, 5, 8): 'Major 7th in 3rd inversion',

(3, 7, 10): 'Minor 7th in root position',
(4, 7, 9): ['Minor 7th in 1st inversion', 'Major Add 6'],
(3, 5, 8): 'Minor 7th in 2nd inversion',
(2, 5, 9): 'Minor 7th in 3rd inversion',

(4, 7, 10): ['Dominant 7th in root position', 'German 6th'],
(3, 6, 8): 'Dominant 7th in 1st inversion',
(3, 5, 9): 'Dominant 7th in 2nd inversion',
(2, 6, 9): 'Dominant 7th in 3rd inversion',

# Inversions for diminished 7ths can't be determined by intervals only.
(3, 6, 9): 'Diminished 7th',

(3, 6, 10): 'Half-Diminished 7th in root position',
(3, 7, 9): ['Half-Diminished 7th in 1st inversion', 'Minor Add 6'],
(4, 6, 9): 'Half-Diminished 7th in 2nd inversion',
(2, 5, 8): 'Half-Diminished 7th in 3rd inversion',

(3, 7, 11): 'Minor Major 7th in root position',
(4, 8, 9): 'Minor Major 7th in 1st inversion',
(4, 5, 8): 'Minor Major 7th in 2nd inversion',
(1, 4, 8): 'Minor Major 7th in 3rd inversion',

(4, 8, 10): 'Augmented 7th chord in root position',
(4, 6, 8): 'Augmented 7th chord in 1st inversion',
(2, 4, 8): 'Augmented 7th chord in 2nd inversion',
(2, 6, 10): 'Augmented 7th chord in 3rd inversion',

(4, 8, 11): 'Augmented Major 7th chord in root position',
(4, 7, 8): 'Augmented Major 7th chord in 1st inversion',
(3, 4, 8): 'Augmented Major 7th chord in 2nd inversion',
(1, 5, 9): 'Augmented Major 7th chord in 3rd inversion',

(4, 6, 10): 'French 6th chord',

(): 'Major Add 2', #FIXME are these Add chords valid?
(): 'Major Add 4',
(): 'Minor Add 2',
(): 'Minor Add 4',

#FIXME must distinguish Extended Chords from Sus2, Sus4, Add6 etc.===
(2, 4, 7, 11): 'Major 9th in root position', #etc.
(2, 7, 11): 'Major 9th in root position with an implied 3rd', #etc

(3, 4, 10): ['Dominant 7th #9 with an implied 5th', 'The Hendrix chord'],
(2, 3, 6, 10): 'Half-Diminished 9th chord in root position',
(2, 4, 7, 9): '6/9 chord',

(6, 7): ['#11 with an implied 3rd', 'The chord of regret'],
(): '11th',
(): '13th',
}

#=================ALL CHORD ANALYSIS BELOW THIS LINE================== 
maj_tri_maj_key = [
'''This is the major I chord: the tonic triad.''',
'''This is the V chord of the V chord.''',
'''Currently Undefined''',
'''This is the major III chord: a standard triad in a major key.''',
'''This chord is borrowed from the relative minor.
It's the major V chord in XXX minor 
(which is actually borrowed from YYY major)''',
'''This is the major IV chord: a standard triad in a major key.''',
'''Currently Undefined''',
'''This is the major V chord: a standard triad in a major key.''',
'''Currently Undefined''',
'''Currently Undefined''',
'''This is a major VII chord which is borrowed from XXX minor''',
'''This is the V chord of the V chord in the relative minor key.''']
