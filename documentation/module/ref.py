# This dict is meant to contain all valid note names and the number of
# semitones above CO. It does not currently include double-flats or
# double-sharps and I'm not even sure this list will be needed.
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

# I don' know how to implement this yet but I figure, once the chord is
# identified, the note names will need to be updated.
accidentals = ['b5', '4+', '#9', '6+', 'etc.']

############################ THE CHORDS DICT ###########################
# The keys for the top-level dict are tuples which contian tuples that
# correspond to the 'integer notation' of the various inversions of the
# chord. Items for these keys are dicts.
    # The keys for this 2nd level of dicts (0~11) correspond to the
    # number of semi-tones above the tonic the ROOT of the chord is.
    # Items for these keys are also dicts.
        # The keys for this 3rd level of dicts correspond to the mode
        # that was set by the user (currently only major and minor).
        # The items for these keys are the descriptions of that
        # particular chord's function at that particular degree of that
        # particular mode.

chords = {'default': 'This chord is not currently defined.',

# INTERVALS ------------------------------------------------------------
(0,): {'default': 'a Unison (the same note)'},
(0, 1): {'default': 'a Semi-tone'},
(0, 2): {'default': 'a Whole Tone'},
(0, 3): {'default': 'a Minor Third'},
(0, 4): {'default': 'a Major Third'},
(0, 5): {'default': 'a Perfect Fourth'},
(0, 6): {'default': 'a Diminished Fifth/Augmented Fourth/Tritone'},
(0, 7): {'default': 'a Perfect Fifth'},
(0, 8): {'default': 'a Minor Sixth or Augmented Fifth'},
(0, 9): {'default': 'a Major Sixth'},
(0, 10): {'default': 'a Minor Seventh or Augmented Sixth'},
(0, 11): {'default': 'a Major Seventh'},

# standard triads ------------------------------------------------------
((0, 7, 12),): {
    'default': 'a Power Chord',
    0: {'major': 'TONIC Power Chord',
        'minor': 'TONIC Power Chord'},
    1: {'major': 'a Neapolitan Power Chord',
        'minor': 'a Neapolitan Power Chord'},
    2: {'major': 'SUPT Power Chord: the V chord of DOM'
                 '\nwhich is the Dominant of TONIC major',
        'minor': 'SUPT Power Chord: the V chord of DOM'
                 '\nwhich is the Dominant of TONIC minor'},
    3: {'major': 'a III Power Chord borrowed from TONIC minor',
        'minor': 'MED Power Chord: a standard Chord in TONIC minor'},
    4: {'major': 'a MED Power Chord borrowed from SUBM minor',
                 '\nwhich is the relative minor of TONIC major'
        'minor': 'a IV Power Chord borrowed from the Neapolitan of MED'
                 '\nwhich is the Relative Major of TONIC minor'},
    5: {'major': 'SUBD Power Chord: a standard chord in TONIC major',
        'minor': 'SUBD Power Chord: a standard chord in TONIC minor'},
    6: {'major': 'a Power Chord on the lowered 5th: Why not?',
        'minor': 'a Power Chord on the lowered 5th: Why not?'},
    7: {'major': 'DOM Power Chord: a standard chord in TONIC major',
        'minor': 'DOM Power Chord: a standard chord in TONIC minor'},
    8: {'major': 'a VI Power Chord borrowed from SUBMm'
                 '\nwhich is the Relative minor of TONIC major',
        'minor': 'SUBM Power Chord: a standard chord in TONIC minor'},
    9: {'major': 'SUBM Power Chord: a standard triad in TONIC major'},
    10:{'major': 'a VII Power Chord borrowed from TONIC minor',
        'minor': 'SUBT Power Chord: a standard chord in TONIC minor'},
    11:{'major': 'a III or III+ chord borrowed from TONIC minor',
        'minor': 'a Power Chord on the leading tone: Why not?'}
    },

((0, 4, 7), (0, 3, 8), (0, 5, 9)): {
    'default': 'a Major Triad INV',
    0: {'major': 'TONIC major (I) INV',
        'minor': 'TONIC major (I) INV '
                 '\n(borrowed from TONIC major)'},
    1: {'major': 'a Neapolitan Chord (N) INV',
        'minor': 'a Neapolitan Chord (N) INV'},
    2: {'major': 'SUPT major is the V chord of DOM'
                 '\nwhich is the Dominant of TONIC major',
        'minor': 'SUPT major is the V chord of DOM'
                 '\nwhich is the Dominant of TONIC minor'},
    3: {'major': 'a III chord INV'
                 '\n(borrowed from TONIC minor)',
        'minor': 'MED major (III) INV: '
                 '\na standard chord in TONIC minor'},
    4: {'major': 'a III chord INV '
                 '\n(borrowed from SUBMm '
                 'which is the relative minor of TONIC major)',
        'minor': 'a IV chord INV'
                 '\n(borrowed from the Neapolitan of MED '
                 'which is the Relative Major of TONIC minor)'},
    5: {'major': 'SUBD (IV) INV:'
                 '\na standard triad in TONIC major'},
    6: {'minor': 'a major chord on the lowered 5th? I don\'t know. '
                 '\nMaybe it\'s borrowed from MED minor'
                 '(MED major being the relative major)'},
    7: {'major': 'DOM (V) INV:'
                 '\na standard triad in TONIC major',
        'minor': 'DOM (V) INV'
                 '\n(a very common alternate chord in TONIC minor)'},
    8: {'minor': 'SUBM major (VI) INV:'
                 '\na standard chord in TONIC minor'},
    10:{'major': 'a VII chord INV'
                 '\n(borrowed from TONIC minor)',
        'minor': 'SUBT major (VII) INV:'
                 '\na standard chord in TONIC minor'},
    11:{'major': 'a III chord INV'
                 '\n(borrowed from TONIC minor)'}
    },

((0, 3, 7), (0, 4, 9), (0, 5, 8)): {
    'default': 'a Minor Triad INV',
    0: {'minor': 'TONICm (i) INV'},
    2: {'major': 'SUPTm (ii) INV:'
                 '\na standard triad in TONIC major',
        'minor': 'SUPT dim (ii dim) INV:'
                 '\na standard triad in TONIC minor'},
    4: {'major': 'MEDm (iii) INV:'
                 '\na standard triad in TONIC major'},
    5: {'major': 'SUBMm (iv) INV'
                 '\n(borrowed from TONIC minor)',
        'minor': 'SUBDm (iv) INV:'
                 '\na standard triad in TONIC minor'},
    6: {'major': 'iv dim INV: the vii dim of DOM'
                 '\n(which is the Dominant of TONIC major)'},
    8: {'major': 'vi dim INV: the vii dim of SUBM'
                 '\n(which is the relative minor of TONIC major)',
        'minor': 'DOMm (v) INV:'
                 '\na standard triad in TONIC minor'},
    9: {'major': 'SUBMm (vi) INV:'
                 '\na standard triad in TONIC major',
        'minor': 'SUBM dim (vi dim) INV: the ii dim of DOM'
                 '\n(which is the (v) of TONIC minor'},
    11:{'major': 'SUBT dim (vii dim) INV:'
                 '\na standard triad in TONIC major',
        'minor': 'SUBT dim (vii dim) INV:'
                 '\na very common alternate chord in TONIC minor'},
    },

((0, 3, 6), (0, 3, 8), (0, 6, 9)): {
    'default': 'a Diminished Triad INV',
    2: {'major': 'SUPT dim (ii dim) INV'
                 '\n(borrowed from TONIC minor)',
        'minor': 'SUPT dim (ii dim) INV:'
                 '\na standard triad in TONIC minor'},
    6: {'major': 'the vii dim chord, INV, of DOM'
                 '\nwhich is the Dominant of TONIC major',
        'minor': 'the vii dim chord, INV, of DOM major'
                 '\nwhich is the Dominant V of TONIC minor'},
    8: {'major': 'a vii dim chord, INV, borrowed from SUBM'
                 '\n(SUBMm being the relative minor of TONIC major)'},
    9: {'minor': 'a ii dim chord, INV, borrowed from DOMm'
                 '\n(which is the v chord of TONICm)'},
    11:{'major': 'SUBT dim (vii dim) INV:'
                 '\na standard triad in TONIC major',
        'minor': 'a vii dim chord, INV, borrowed from MED major'
                 '\n(which is the Relative Major of TONICm)'}
    },

# Altered triads -------------------------------------------------------

# Inversions for augmented triads can't be determined by intervals only.
((0, 4, 8),): {
    'default': 'an Augmented Triad INV',
    }, 

((0, 2, 7), (0, 5, 10), (0, 5, 7)): {
    'default': 'a Sus 2 INV'
    },


((0, 4, 5), (0, 1, 3), (0, 7, 11)): {
    'default': 'a Sus 4 INV'
    },


((0, 4, 10),): {
    'default': 'an Italian VI+ chord INV',
    8: {'major': 'an Italian VI+ chord',
        'minor': 'an Italian VI+ chord'}
    },

((0, 4, 11), (0, 7, 8), None, (0, 1, 5)): {
    'default': 'a Major 7th with an implied 5th INV',
    0: {'major': 'TONIC Maj7 (I Maj7) INV'
                 '\nwith an implied 5th'},
    3: {'major': 'a MED Maj7 (III Maj7) INV,'
                 '\nwith an implied 5th'
                 '\n(borrowed from the minor key)',
        'minor': 'MED Maj7 (III Maj7) INV,'
                 '\nwith an implied 5th:'
                 '\na standard 7th chord in TONIC minor'},
    5: {'major': 'SUBD Maj7 (IV Maj7) INV,'
                 '\nwith an implied 5th:'
                 '\na standard 7th chord in TONIC major'},
    8: {'minor': 'SUBM Maj7 (VI Maj7) INV,'
                 '\nwith an implied 5th:'
                 '\na standard 7th chord in TONIC minor'},
    },

((0, 3, 10), (0, 7, 9), None, (0, 2, 5)): {
    'default': 'a Minor 7th with an implied 5th INV'
    },

((0, 7, 10), (0, 3, 8), None, (0, 2, 6)): {
    'default': 'a Dominant 7th with an implied 5th INV'
    },

# A diminished 5th should not be implied

((0, 3, 11), (0, 8, 9), None, (0, 1, 4)): {
    'default': 'a Minor Major 7th with an implied 5th INV'
    },

# An augmented 5th should not be implied

# standard 7th chords --------------------------------------------------
((0, 4, 7, 11), (0, 3, 7, 8), (0, 4, 5, 9), (0, 1, 5, 8)): {
    'default': 'a Major 7th INV',
    0: {'major': 'TONIC Maj7 (I Maj7) INV'},
    3: {'major': 'a MED Maj7 (III Maj7) INV,'
                 '\n(borrowed from the minor key)',
        'minor': 'MED Maj7 (III Maj7) INV:'
                 '\na standard 7th chord in TONIC minor'},
    5: {'major': 'SUBD Maj7 (IV Maj7) INV:'
                 '\na standard 7th chord in TONIC major'},
    8: {'minor': 'SUBM Maj7 (VI Maj7) INV:'
                 '\na standard 7th chord in TONIC minor'},
    },

((0, 3, 7, 10), (0, 4, 7, 9), (0, 3, 5, 8), (0, 2, 5, 9)): {
    'default': 'a Minor 7th INV',
    0: {'minor': 'TONICm7 (Im7) INV'},
    2: {'major': 'SUPTm7 (ii7) INV:'
                 '\na standard 7th chord in TONIC major'},
    4: {'major': 'MEDm7 (iii7) INV:'
                 '\na standard 7th chord in TONIC major'},
    5: {'major': 'a SUBDm7 (iv7) INV'
                 '\n(borrowed from the minor key)',
        'minor': 'SUBDm7 (iv7) INV:'
                 '\na standard 7th chord in TONIC major'},
    7: {'minor': 'DOMm7 (v7) INV:'
                 '\na standard 7th chord in TONIC minor'},
    9: {'major': 'SUBMm7 (vi7) INV:'
                 '\na standard 7th chord in TONIC major'},
    },

((0, 4, 7, 10), (0, 3, 6, 8), (0, 3, 5, 9), (0, 2, 6, 9)): {
    'default': 'a Dominant 7th INV',
    1: {'major': 'a Neapolitan 7th (N7) INV',
        'minor': 'a Neapolitan 7th (N7) INV'},
    2: {'minor': 'a SUPT Dom7 (II7) INV:'
                 '\nthe V7 of DOM'
                 '\n(which is the Dominant of TONIC major)'},
    4: {'major': 'MED Dom7 (III Dom7) INV:'
                 '\n(a borrowed V7 of DOM)'},
    7: {'major': 'DOM7 (V7) INV:'
                 '\na standard 7th chord in TONIC major',
        'minor': 'DOM Dom7 (V7) INV:'
                 '\na very common alternate chordin TONIC minor'},
    8: {'major': 'a German VI+ INV'},
    10:{'major': 'a VII7 INV'
                 '\n(borrowed from TONIC minor)',
        'minor': 'SUBT7 (VII7) INV:'
                 '\na standard 7th chord in TONIC minor'}
    },

((0, 3, 6, 10), (0, 3, 7, 9), (0, 4, 6, 9), (0, 2, 5, 8)): {
    'default': 'a Half-Diminished 7th INV',
    2: {'major': 'SUPT 1/2dim7 (ii 1/2dim7) INV'
                 '(borrowed from TONIC minor)',
        'minor': 'SUPT 1/2dim7 (ii 1/2dim7) INV:'
                 '\na standard 7th chord in TONIC minor'},
    6: {'major': 'the vii dim7 chord, INV, of DOM '#???
                 '\n(which is the Dominant of TONIC major)',
        'minor': 'the vii dim7 chord, INV, of DOM major '#???
                 '\n(which is the Dominant V of TONIC minor)'},
    8: {'major': 'a vii dim7 chord, INV, borrowed from SUBM '#???
                 '\n(SUBMm being the relative minor of TONIC major)'},
    9: {'minor': 'a ii 1/2dim7 chord, INV, borrowed from DOMm'
                 '\n(which is the v chord of TONICm)'},
    11:{'major': 'SUBT 1/2dim7 (vii 1/2dim7) INV: '
                 '\na standard 7th chord in TONIC major',
        'minor': 'a vii 1/2dim7 chord, INV, borrowed from MED major'
                 '\n(which is the Relative Major of TONICm)'}
    },

# altered 7th chords ---------------------------------------------------

# Inversions for diminished 7ths can't be determined by intervals only.
((0, 3, 6, 9),): {
    'default': 'a Diminished 7th INV',
    6: {'major': 'the vii dim chord, INV, of DOM'#???
                 '\n(which is the Dominant of TONIC major)',
        'minor': 'the vii dim chord, INV, of DOM major'#???
                 '\n(which is the Dominant V of TONIC minor)'},
    8: {'major': 'a vii dim chord, INV, borrowed from SUBM'#???
                 '\n(SUBMm being the relative minor of TONIC major)'}
    },

((0, 3, 7, 11), (0, 4, 8, 9), (0, 4, 5, 8), (0, 1, 4, 8)): {
    'default': 'a Minor Major 7th INV'
    },

((0, 4, 8, 10), (0, 4, 6, 8), (0, 2, 4, 8), (0, 2, 6, 10)): {
    'default': 'an Augmented 7th INV'
    },

((0, 4, 8, 11), (0, 4, 7, 8), (0, 3, 4, 8), (0, 1, 5, 9)): {
    'default': 'an Augmented Major 7th INV'
    },

((0, 4, 6, 10),): {
    'default': 'a Dominant 7th flat 5 INV',
    8: {'major': 'French 6th chord INV',
        'minor': 'French 6th chord INV'}
    },

# extended chords ------------------------------------------------------

#FIXME must distinguish Extended Chords from Sus2, Sus4, Add6 etc.
((0, 2, 4, 7, 11),): {
    'default': 'a Major 9th INV'
    },

((0, 2, 7, 11),): {
    'default': 'a Major 9th with an implied 3rd INV'
    },

((0, 3, 4, 10),): {
    'default': 'a Dominant 7th #9 with an implied 5th INV',
    0: {'major': 'The Hendrix chord INV'}
    },
    
((0, 2, 3, 6, 10),): {
    'default': 'a Half-Diminished 9th INV'
    },
    
((0, 2, 4, 7, 9),): {
    'default': 'a 6/9 chord INV'
    },

((0, 6, 7),): {
    'default': 'a #11 with an implied 3rd INV',
    0: {'minor': 'The chord of regret INV'
        }
    }
}
