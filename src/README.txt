########## BEND - version 0.04 ##########


===== About This Version =====

NEW: Bend now reliably takes any valid key and returns valid
information including:
    Scale Degree of a note
    The number of semitones above the tonic a note is
    The interval of 2 notes
    The number of semitones in the interval
    An analysis of chords containing 3 or more notes

However, the dictionary from which this information comes is still
extremely limited. 

This version can only be run in the terminal.
It prompts the user for a key which must be entered as the
tonic note (F#) followed by a space and mode (major, minor).
EXAMPLE $ Bb major
('b' represents the symbol for 'flat')

The user is then prompted for a note which must be entered in
the same format as the key but with a range instead of a mode.
EXAMPLE $ C 3
(don't forget the space)

The user will continue to be prompted and information will continue to
be returned for any chord currently in the dictionary.


===== To Run From Terminal Without installing =====
$ python YOURPATH/bend


===== To Install =====
If you don't have setuptools:
download https://bitbucket.org/pypa/setuptools/raw/bootstrap/ez_setup.py
and run this command from the containing directory:
$ sudo python ez_setup.py

If you don't have pip:
download https://raw.github.com/pypa/pip/master/contrib/get-pip.py
and run this command from the containing directory:
$ sudo python get-pip.py

Then:
$ sudo pip install nose

Then, from the 0.04 directory:
$ sudo python setup.py install

# Now BEND should run from any terminal window by typing only:
$ bend 


===== To Uninstall =====
$ sudo pip uninstall bend


========== MISSION ==========

Bend aims to be a resource for identifying chords and the functions they
serve in music. Bend will receive input from a GUI or MIDI messages.

There are several programs available for identifying chords, but they
generally provide only the technical name of the chord and limited
(if any) information about how the chord is used.

Bend is intended to provide composers and tunesmiths with an immediate
and intuitive way to reference helpful information about chord
structures and progressions so that they can concentrate on creating
more interesting and expressive music.
Information will include:
    * The technical name of a chord ( F# half-diminished 7 )
    * The Universal Key representation ( ii@7 )
    * Enharmonic equivalents ( C Add6 )
    * Chords that most commonly precede and follow it ( B7 )
    * The key it's borrowed from (if borrowed)
    * Keys that the chord can be used to modulate to ( G . . . )
This program is not intended for atonal music, or music which requires
more than 12 tones per octave.


# TODOS ##################################################################

# Chord class should convert or at least display (#9, b5, +6 etc)
# user can set a preference of visual aides: piano, guitar, clef, etc.
# when key entered, list other relevant keys (relative minor, fifth etc)
# when 3rd note is entered
    # same note is ignored in analysis (unless it's the new bass)
    # determine the root and quality of the chord
    # return
        # the chord
        # a prominent roman numeral representation
        # chords that commonly precede and follow it
        # the key it's borrowed from (if borrowed)
        # keys it can be used to modulate to
        # a detailed description of function
        # various representations (constellations, integer notation etc)
        # link to further information
        # all other chords which contain the same intervals

# if user follows a modulation, change the key signature automatically
# auto-update and notify user if chord type changes note name

# UNICODE:
	# flat = 266D, double flat = 1D12B,
	# sharp = 266F, double sharp = 1D12A
	# diminished = 004F, half-diminished = 00D8

#The following chords need to be added:
	# Dom VII7b5
	# All 'altered' chords
	# Aug 7th (altered dominant?)
	# Minor/Major on the tonic
	# Neapolitan borrowed from the relative major.
